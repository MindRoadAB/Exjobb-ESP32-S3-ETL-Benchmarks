import json
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

with open('./results/benchmark_results.json') as f:
    data = json.load(f)

# Flatten JSON into a DataFrame
rows = []
for context, operations in data.items():
    ctx_parts = dict(part.split('=') for part in context.split(','))
    for op, cycles in operations.items():
        rows.append({
            'USE_ETL': int(ctx_parts['USE_ETL']),
            'TYPE': ctx_parts['TYPE'],
            'OPERATION': op,
            'CYCLES': cycles
        })

df = pd.DataFrame(rows)

output_dir = Path('./results/plots')
output_dir.mkdir(exist_ok=True)

# Loop over each TYPE and generate plots
for target_type in df['TYPE'].unique():
    subset = df[df['TYPE'] == target_type]

    pivot = subset.pivot(index='OPERATION', columns='USE_ETL', values='CYCLES')

    # Keep only operations that exist in both ETL modes
    pivot = pivot.dropna()

    if pivot.empty:
        print(f"⚠️ Skipping '{target_type}' — no common operations.")
        continue

    pivot = pivot.sort_values(by=0)

    # Plot
    ax = pivot.plot(kind='bar', figsize=(14, 6))
    ax.set_yscale('log')
    ax.yaxis.grid(True, which='both', linestyle='--', linewidth=0.5)

    plt.title(f'{target_type} Benchmarks (ETL vs STL)')
    plt.ylabel('Cycles per iteration (log scale)')
    plt.xlabel('Operation')
    plt.xticks(rotation=45, ha='right')
    plt.legend(title='USE_ETL', labels=['STL', 'ETL'])
    plt.tight_layout()

    out_path = output_dir / f'bench_{target_type}.png'
    plt.savefig(out_path)
    print(f"✅ Saved: {out_path}")

    plt.show()


