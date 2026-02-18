import xml.etree.ElementTree as ET
import csv
import os

# 1. Lista de archivos XML (ajusta el patrón si hace falta)
xml_files = [os.path.join('../../corpus/proyecto/ptrain/', f) for f in os.listdir('../../corpus/proyecto/ptrain/') if f.endswith('.xml')]

print("Voy a procesar:", xml_files)

with open("tweets.csv", "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f)
    # Escribimos el header UNA sola vez
    writer.writerow(["tweetid", "content", "sentiment"])

    # 2. Recorremos cada XML
    for xml_path in xml_files:
        tree = ET.parse(xml_path)
        root = tree.getroot()

        # 3. Recorremos cada <tweet> en este XML
        for tweet in root.findall(".//tweet"):
            tweetid = tweet.findtext("tweetid", "").strip()

            content = tweet.findtext("content", "")
            if content is not None:
                content = " ".join(content.split())  # limpia saltos de línea/espacios raros
            else:
                content = ""

            value_elem = tweet.find("sentiment/polarity/value")
            sentiment = value_elem.text.strip() if value_elem is not None else ""

            writer.writerow([tweetid, content, sentiment])
