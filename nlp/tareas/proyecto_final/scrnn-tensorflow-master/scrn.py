# SCRNCell in PyTorch
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.utils.tensorboard import SummaryWriter
from torch.utils.data import Dataset, DataLoader
import numpy as np
import io
import argparse
import time


class SCRNCell(nn.Module):
    def __init__(self, input_size, hidden_size, context_size, alpha=0.95):
        super(SCRNCell, self).__init__()
        self.hidden_size = hidden_size
        self.context_size = context_size
        self.alpha = alpha

        self.B = nn.Parameter(torch.Tensor(input_size, context_size))
        self.V = nn.Parameter(torch.Tensor(context_size, hidden_size))
        self.U = nn.Parameter(torch.Tensor(hidden_size, hidden_size))

        self.linear = nn.Linear(input_size + hidden_size + context_size, hidden_size)
        self.init_parameters()

    def init_parameters(self):
        for name, param in self.named_parameters():
            if param.dim() >= 2:
                nn.init.xavier_uniform_(param)
            else:
                nn.init.zeros_(param)  # inicializa sesgos a cero


    def forward(self, input, hx):
        h_prev, s_prev = hx

        s_t = (1 - self.alpha) * input @ self.B + self.alpha * s_prev
        concat = torch.cat([input, h_prev, s_t], dim=1)
        h_t = torch.sigmoid(self.linear(concat))

        return h_t, (h_t, s_t)


class SCRNModel(nn.Module):
    def __init__(self, vocab_size, input_size, hidden_size, context_size, alpha=0.95):
        super(SCRNModel, self).__init__()
        self.embedding = nn.Embedding(vocab_size, input_size)
        self.rnn_cell = SCRNCell(input_size, hidden_size, context_size, alpha)
        self.fc = nn.Linear(hidden_size, vocab_size)

    def forward(self, x):
        batch_size, seq_len = x.size()
        embedded = self.embedding(x)
        h_t = torch.zeros(batch_size, self.rnn_cell.hidden_size, device=x.device)
        s_t = torch.zeros(batch_size, self.rnn_cell.context_size, device=x.device)

        for t in range(seq_len):
            input_t = embedded[:, t, :]
            h_t, (h_t, s_t) = self.rnn_cell(input_t, (h_t, s_t))

        out = self.fc(h_t)
        return out


class CharDataset(Dataset):
    def __init__(self, text, seq_length, vocab=None):
        text = text.lower()
        self.seq_length = seq_length
        self.vocab = sorted(set(text)) if vocab is None else vocab
        self.char2idx = {ch: idx for idx, ch in enumerate(self.vocab)}
        self.idx2char = {idx: ch for ch, idx in self.char2idx.items()}

        self.encoded = [self.char2idx[c] for c in text if c in self.char2idx]
        self.samples = []

        for i in range(0, len(self.encoded) - seq_length):
            seq = self.encoded[i:i + seq_length]
            target = self.encoded[i + seq_length]
            self.samples.append((torch.tensor(seq), torch.tensor(target)))

    def __len__(self):
        return len(self.samples)

    def __getitem__(self, idx):
        return self.samples[idx]

    def vocab_size(self):
        return len(self.vocab)


def load_text(train_path, test_path):
    with io.open(train_path, encoding='utf-8') as f:
        train_text = f.read().lower()
    with io.open(test_path, encoding='utf-8') as f:
        test_text = f.read().lower()
    return train_text, test_text


def train(model, dataloader, optimizer, criterion, device, epoch, writer):
    model.train()
    print(f"\n===== Comenzando epoca {epoch} =====")
    total_loss = 0
    for i, (x_batch, y_batch) in enumerate(dataloader):
        if i % 100 == 0:
            print(f"Batch {i}/{len(dataloader)}")
        x_batch, y_batch = x_batch.to(device), y_batch.to(device)
        optimizer.zero_grad()
        logits = model(x_batch)
        loss = criterion(logits, y_batch)
        loss.backward()
        optimizer.step()
        total_loss += loss.item()
    avg_loss = total_loss / len(dataloader)
    perplexity = torch.exp(torch.tensor(avg_loss))
    writer.add_scalar("Loss/train", avg_loss, epoch)
    writer.add_scalar("Perplexity/train", perplexity.item(), epoch)
    print(f"Epoch {epoch}: Loss={avg_loss:.4f}, Perplexity={perplexity:.4f}")


def evaluate(model, dataloader, criterion, device):
    model.eval()
    total_loss = 0
    with torch.no_grad():
        for x_batch, y_batch in dataloader:
            x_batch, y_batch = x_batch.to(device), y_batch.to(device)
            logits = model(x_batch)
            loss = criterion(logits, y_batch)
            total_loss += loss.item()
    avg_loss = total_loss / len(dataloader)
    perplexity = torch.exp(torch.tensor(avg_loss))
    print(f"Eval Loss={avg_loss:.4f}, Perplexity={perplexity:.4f}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--train_path', type=str, default='data/ptb/ptb.train.txt')
    parser.add_argument('--test_path', type=str, default='data/ptb/ptb.test.txt')
    parser.add_argument('--seq_length', type=int, default=30)
    parser.add_argument('--batch_size', type=int, default=32)
    parser.add_argument('--input_size', type=int, default=64)
    parser.add_argument('--rnn_size', type=int, default=64)
    parser.add_argument('--context_size', type=int, default=40)
    parser.add_argument('--alpha', type=float, default=0.95)
    parser.add_argument('--lr', type=float, default=1e-1)
    parser.add_argument('--num_epoch', type=int, default=100)
    args = parser.parse_args()

    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    train_text, test_text = load_text(args.train_path, args.test_path)
    train_dataset = CharDataset(train_text, args.seq_length)
    test_dataset = CharDataset(test_text, args.seq_length, vocab=train_dataset.vocab)

    train_loader = DataLoader(train_dataset, batch_size=args.batch_size, shuffle=True)
    test_loader = DataLoader(test_dataset, batch_size=args.batch_size)

    model = SCRNModel(vocab_size=len(train_dataset.vocab), input_size=args.input_size,
                      hidden_size=args.rnn_size, context_size=args.context_size,
                      alpha=args.alpha)
    model.to(device)

    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(model.parameters(), lr=args.lr)
    writer = SummaryWriter()

    for epoch in range(args.num_epoch):
        train(model, train_loader, optimizer, criterion, device, epoch, writer)
        if epoch % 10 == 0 and epoch > 0:
            evaluate(model, test_loader, criterion, device)

    writer.close()

    # Guardar modelo
    torch.save(model.state_dict(), "scrn_model.pt")
    import pickle
    with open("vocab.pkl", "wb") as f:
        pickle.dump(train_dataset.char2idx, f)



if __name__ == '__main__':
    main()
