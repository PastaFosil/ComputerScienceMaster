## Libraries used accelerate transformers datasets accelerate peft trl bitsandbytes qqq dlash-attn

import torch

if torch.cuda.get_device_capability()[0] >= 8:
    attn_implementation = "flash_attention_2"
    torch_dtype = torch.bfloat16
else:
    attn_implementation = "eager"
    torch_dtype = torch.float16

import gc
import os
import pickle
from datasets import load_dataset
from peft import LoraConfig, PeftModel, prepare_model_for_kbit_training
#from tqdm import tqdm

from transformers import (
    AutoModelForCausalLM,
    AutoTokenizer,
    BitsAndBytesConfig,
    TrainingArguments,
    pipeline,
)
from trl import ORPOConfig, ORPOTrainer, setup_chat_format

os.environ["HF_TOKEN"] = "" ##Your hugging face token to access the model

base_model = "meta-llama/Llama-3.2-3B"

# QLoRA configuration
bnb_config = BitsAndBytesConfig(
    load_in_4bit=True,
    bnb_4bit_quant_type="nf4",
    bnb_4bit_compute_dtype=torch_dtype,
    bnb_4bit_use_double_quant=True,
)

# Load model
model = AutoModelForCausalLM.from_pretrained(
    base_model,
    quantization_config=bnb_config,
    device_map="auto",
    attn_implementation=attn_implementation
)
tokenizer = AutoTokenizer.from_pretrained(base_model, pad_side = "left")
model, tokenizer = setup_chat_format(model, tokenizer)

#read data
file_path = "paginas_limpias/controlar_la_corrupcion_no_es_opcional"
with open(file_path, "r", encoding="utf-8") as fh:
  input_text = fh.read()

print("\n\nTEXT:\n\n")
print(input_text[:1000])
print("\n\n\n\n")

#if memory staurates, garbage collector can be programmed periodically
def collect_garbage(input_message, output):
  del input_message
  del output
  gc.collect()
  gc.collect()
  torch.cuda.empty_cache()

#Prompt for positive and negative explanations
## Special tokens needed as we are using chat format // instruct llama
def evaluate_corruption(page):
  messages = f"""<|begin_of_text|>
  <|start_header_id|>system<|end_header_id|>You are a specialist on acts corruption. Think step by step.<|eot_id|>
  <|start_header_id|>user<|end_header_id|>Tell me briefly what the following text is about.\nText: {page}<|eot_id|>
  <|start_header_id|>assistant<|end_header_id|>"""
  return messages

tokenizer.padding_side="left"
batch_size = 64

pipe = pipeline("text-generation", model=model, tokenizer=tokenizer, device_map="auto")

# Generar
output = pipe(input_text, max_new_tokens=100, do_sample=True, top_k=50, temperature=0.8)

# Mostrar resultado
print(output[0]["generated_text"])
