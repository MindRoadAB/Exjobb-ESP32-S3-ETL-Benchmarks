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

        fig, ax = plt.subplots(figsize=(18, 6))
        ax.plot(heap_df['timestamp'], heap_df['ratio'], label='heap ratio', linewidth=1.5)

        for _, row in event_df.iterrows():
            ax.axvline(x=row['timestamp'], color='red', linestyle='--', alpha=0.6)

        ax.set_title(f'Heap Ratio Over Time (ETL={etl}, STATIC={static})')
        ax.set_xlabel('Time (s)')
        ax.set_ylabel('Ratio (largest / free)')
        ax.grid(True)
        fig.tight_layout()

        out_file = RESULTS_DIR / f'heap_plot_ETL{etl}_STATIC{static}.png'
        fig.savefig(out_file)
        plt.close(fig)

        print(f"✅ Saved: {out_file}")

        # Free heap over time plot
        fig, ax = plt.subplots(figsize=(18, 6))
        ax.plot(heap_df['timestamp'], heap_df['free_heap'], label='free_heap', color='blue', linewidth=1.5)
        ax.set_title(f'Free Heap Over Time (ETL={etl}, STATIC={static})')
        ax.set_xlabel('Time (s)')
        ax.set_ylabel('Free Heap (bytes)')
        ax.grid(True)
        fig.tight_layout()

        out_file = RESULTS_DIR / f'free_heap_ETL{etl}_STATIC{static}.png'
        fig.savefig(out_file)
        plt.close(fig)
        print(f"✅ Saved: {out_file}")

        # Largest block over time plot
        fig, ax = plt.subplots(figsize=(18, 6))
        ax.plot(heap_df['timestamp'], heap_df['largest_block'], label='largest_block', color='green', linewidth=1.5)
        ax.set_title(f'Largest Free Block Over Time (ETL={etl}, STATIC={static})')
        ax.set_xlabel('Time (s)')
        ax.set_ylabel('Largest Block (bytes)')
        ax.grid(True)
        fig.tight_layout()

        out_file = RESULTS_DIR / f'largest_block_ETL{etl}_STATIC{static}.png'
        fig.savefig(out_file)
        plt.close(fig)
        print(f"✅ Saved: {out_file}")