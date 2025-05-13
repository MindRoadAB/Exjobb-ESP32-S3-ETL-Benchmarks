import json
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

# Load JSON
with open('./results/benchmark_results.json') as f:
    data = json.load(f)

# Flatten into DataFrame
rows = []
for ctx_str, ops in data.items():
    ctx = dict(part.split('=') for part in ctx_str.split(','))
    for op, cycles in ops.items():
        rows.append({
            'TYPE': ctx['TYPE'],
            'USE_ETL': int(ctx['USE_ETL']),
            'OPERATION': op,
            'CYCLES': cycles
        })
df = pd.DataFrame(rows)

# Pivot
pivot = df.pivot_table(index=['TYPE', 'OPERATION'], columns='USE_ETL', values='CYCLES')
pivot.columns = ['STL', 'ETL']
pivot = pivot.dropna()
pivot['DIFF'] = pivot['ETL'] - pivot['STL']
pivot['REL_CHANGE_%'] = (pivot['ETL'] / pivot['STL'] - 1) * 100

# Output directory
Path('./results/png').mkdir(parents=True, exist_ok=True)

# Per-type plots
for t in pivot.index.get_level_values('TYPE').unique():
    subset = pivot.loc[t].copy()
    subset = subset.sort_values('REL_CHANGE_%', ascending=True)

    plt.figure(figsize=(10, max(6, 0.3 * len(subset))))
    bars = plt.barh(subset.index, subset['REL_CHANGE_%'],
                    color=['green' if x < 0 else 'red' for x in subset['REL_CHANGE_%']])

    plt.axvline(0, color='black', linestyle='--', linewidth=1)
    plt.title(f'ETL vs libstdc++: {t}')
    plt.xlabel('Relative Change (%)  (negative = ETL faster)')
    plt.tight_layout()

    out_file = f'./results/png/{t}_etl_vs_stl.png'
    plt.savefig(out_file, dpi=200)
    print(f"✅ Saved {out_file}")
    plt.close()
