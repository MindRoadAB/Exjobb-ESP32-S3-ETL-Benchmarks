import json
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

# Constants
INPUT_JSON = './results/benchmark_comparison.json'
OUTPUT_DIR = Path('./results/plots')
OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

# Load JSON
with open(INPUT_JSON) as f:
    raw_data = json.load(f)

# Flatten the structure
rows = []
for context_key, operations in raw_data.items():
    parts = dict(part.split('=') for part in context_key.split(','))
    use_etl = int(parts['USE_ETL'])
    lib = 'ETL' if use_etl == 1 else 'STL'
    type_ = parts['TYPE']

    for op, metrics in operations.items():
        rows.append({
            'TYPE': type_,
            'LIB': lib,
            'OPERATION': op,
            'non_optimized': metrics.get('non_optimized', None),
            'optimized': metrics.get('optimized', None),
            'percent_improvement': metrics.get('percent_improvement', None)
        })

df = pd.DataFrame(rows)

# Generate one plot per TYPE
for type_ in df['TYPE'].unique():
    subset = df[df['TYPE'] == type_]

    # Pivot the data: operations as index, columns = LIB + metric
    pivot = subset.pivot(index='OPERATION', columns='LIB')[['non_optimized', 'optimized', 'percent_improvement']]
    pivot.columns = [f'{lib}_{metric}' for metric, lib in pivot.columns]
    pivot = pivot.dropna()

    if pivot.empty:
        print(f"⚠️ No valid data for {type_}")
        continue

    pivot = pivot.sort_index()

    ax = pivot.plot(kind='bar', figsize=(18, 7))
    ax.set_title(f'{type_} — Optimization Benchmark (ETL vs STL)')
    ax.set_ylabel('Cycles (or % for improvement)')
    ax.set_xlabel('Operation')
    ax.set_yscale('log')
    ax.grid(True, axis='y', linestyle='--', linewidth=0.5)
    plt.xticks(rotation=45, ha='right')
    plt.tight_layout()

    output_file = OUTPUT_DIR / f'comparison_{type_}.png'
    plt.savefig(output_file)
    print(f'✅ Saved plot to: {output_file}')
    plt.close()
