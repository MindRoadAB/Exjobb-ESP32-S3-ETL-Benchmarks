import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

RESULTS_DIR = Path('results')

for etl in [0, 1]:
    for static in [0, 1]:
        heap_file = RESULTS_DIR / f'heap_data_USE_ETL={etl}_USE_STATIC={static}.csv'
        event_file = RESULTS_DIR / f'heap_events_USE_ETL={etl}_USE_STATIC={static}.csv'

        heap_df = pd.read_csv(heap_file)
        event_df = pd.read_csv(event_file)

        plt.figure(figsize=(12, 6))
        plt.plot(heap_df['timestamp'], heap_df['ratio'], label='heap ratio', linewidth=1.5)

        for _, row in event_df.iterrows():
            plt.axvline(x=row['timestamp'], color='red', linestyle='--', alpha=0.6)
            plt.text(row['timestamp'], 0.95, row['event'], rotation=90, fontsize=8, va='top', ha='right', color='red')

        plt.title(f'Heap Ratio Over Time (ETL={etl}, STATIC={static})')
        plt.xlabel('Time (s)')
        plt.ylabel('Ratio (largest / free)')
        plt.grid(True)
        plt.tight_layout()

        out_file = RESULTS_DIR / f'heap_plot_ETL{etl}_STATIC{static}.png'
        plt.savefig(out_file)
        plt.close()

        print(f"✅ Saved: {out_file}")
