import json
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# === CONFIG ===
NON_OPT_FILE = "results/benchmark_results.json"
OPT_FILE = "results/benchmark_results-optimized.json"
OUTPUT_DIR = "results/plots"

os.makedirs(OUTPUT_DIR, exist_ok=True)

# === LOAD DATA ===
def extract_data(file_path):
    with open(file_path, "r") as f:
        raw = json.load(f)

    data = []
    for key, func_dict in raw.items():
        etl_flag = "ETL" if "USE_ETL=1" in key else "STD"
        type_part = [p for p in key.split(",") if p.startswith("TYPE=")]
        if not type_part:
            continue
        type_name = type_part[0].split("=")[1].strip()

        for func_name, value in func_dict.items():
            data.append({
                "Function": func_name,
                "TYPE": type_name,
                "ETL_FLAG": etl_flag,
                "Value": value
            })
    return pd.DataFrame(data)

# Load both datasets
non_opt_df = extract_data(NON_OPT_FILE)
opt_df = extract_data(OPT_FILE)

# Combine and label
non_opt_df["OPTIMIZED"] = False
opt_df["OPTIMIZED"] = True
all_data = pd.concat([non_opt_df, opt_df], ignore_index=True)

# === PROCESS COMPARISONS ===
results = []

grouped = all_data.groupby(["Function", "TYPE", "OPTIMIZED"])

for (func, typ, opt), group in grouped:
    if {"ETL", "STD"}.issubset(set(group["ETL_FLAG"])):
        std_val = group[group["ETL_FLAG"] == "STD"]["Value"].values[0]
        etl_val = group[group["ETL_FLAG"] == "ETL"]["Value"].values[0]
        if std_val == 0:
            continue
        pct_improvement = 100 * (std_val - etl_val) / std_val
        results.append({
            "Function": func,
            "TYPE": typ,
            "OPTIMIZED": opt,
            "STD": std_val,
            "ETL": etl_val,
            "% Improvement": pct_improvement
        })

df = pd.DataFrame(results)

# === HEATMAP ===
def plot_heatmap(df, optimized_flag):
    subset = df[df["OPTIMIZED"] == optimized_flag]
    pivot = subset.pivot_table(index="Function", columns="TYPE", values="% Improvement")
    fig, ax = plt.subplots(figsize=(12, 8))
    sns.heatmap(pivot, annot=False, cmap="RdYlGn", center=0, linewidths=0.5,
                cbar_kws={'label': '% Improvement (ETL over STD)'}, ax=ax)
    opt_label = "optimized" if optimized_flag else "non_optimized"
    ax.set_title(f"ETL vs STD % Improvement Heatmap ({opt_label})")
    fig.tight_layout()
    filename = os.path.join(OUTPUT_DIR, f"heatmap_{opt_label}.png")
    fig.savefig(filename)
    plt.close(fig)

# === SCATTER PLOT ===
def plot_scatter(df, optimized_flag):
    subset = df[df["OPTIMIZED"] == optimized_flag]
    fig, ax = plt.subplots(figsize=(14, 6))
    sns.scatterplot(data=subset, x="Function", y="% Improvement", hue="TYPE", s=100, ax=ax)
    ax.axhline(0, color='gray', linestyle='--')
    ax.set_xticklabels(ax.get_xticklabels(), rotation=90)
    opt_label = "optimized" if optimized_flag else "non_optimized"
    ax.set_title(f"ETL vs STD % Improvement Scatter Plot ({opt_label})")
    fig.tight_layout()
    filename = os.path.join(OUTPUT_DIR, f"scatter_{opt_label}.png")
    fig.savefig(filename)
    plt.close(fig)

# === RUN ===
plot_heatmap(df, optimized_flag=False)
plot_heatmap(df, optimized_flag=True)
plot_scatter(df, optimized_flag=False)
plot_scatter(df, optimized_flag=True)

print(f"Plots saved to '{OUTPUT_DIR}/'")
