import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

RESULTS_DIR = Path('results')
colors = ['blue', 'orange', 'green', 'purple']
labels = []

heap_data = {}
event_data = {}

# Load all four versions
for i, (etl, static) in enumerate([(0, 0), (0, 1), (1, 0), (1, 1)]):
    key = f'ETL={etl}, STATIC={static}'
    labels.append(key)
    heap_file = RESULTS_DIR / f'heap_data_USE_ETL={etl}_USE_STATIC={static}-vector-60min-noresize.csv'
    event_file = RESULTS_DIR / f'heap_events_USE_ETL={etl}_USE_STATIC={static}-vector-60min-noresize.csv'

    heap_data[key] = pd.read_csv(heap_file)
    event_data[key] = pd.read_csv(event_file)

# === Plot 1: Heap Ratio Over Time ===
fig, ax = plt.subplots(figsize=(18, 6))

for i, key in enumerate(heap_data.keys()):
    df = heap_data[key]
    ax.plot(df['timestamp'], df['ratio'], label=labels[i], color=colors[i], linewidth=1.5)

ax.set_title('Heap Ratio Over Time (All Configs)')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Ratio (largest / free)')
ax.legend()
ax.grid(True)
fig.tight_layout()
fig.savefig(RESULTS_DIR / 'heap_ratio_all_configs-vector-60min-noresize-s3.png')
plt.close(fig)
print("✅ Saved: heap_ratio_all_configs.png")

# === Plot 2: Free Heap Over Time ===
fig, ax = plt.subplots(figsize=(18, 6))

for i, key in enumerate(heap_data.keys()):
    df = heap_data[key]
    ax.plot(df['timestamp'], df['free_heap'], label=labels[i], color=colors[i], linewidth=1.5)

ax.set_title('Free Heap Over Time (All Configs)')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Free Heap (bytes)')
ax.legend()
ax.grid(True)
fig.tight_layout()
fig.savefig(RESULTS_DIR / 'free_heap_all_configs-vector-60min-noresize-s3.png')
plt.close(fig)
print("✅ Saved: free_heap_all_configs.png")

# === Plot 3: Largest Block Over Time ===
fig, ax = plt.subplots(figsize=(18, 6))

for i, key in enumerate(heap_data.keys()):
    df = heap_data[key]
    ax.plot(df['timestamp'], df['largest_block'], label=labels[i], color=colors[i], linewidth=1.5)

ax.set_title('Largest Block Over Time (All Configs)')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Largest Block (bytes)')
ax.legend()
ax.grid(True)
fig.tight_layout()
fig.savefig(RESULTS_DIR / 'largest_block_all_configs-vector-60min-noresize-s3.png')
plt.close(fig)
print("✅ Saved: largest_block_all_configs.png")

# === Plot 4: Fragmentation Ratio Over Time ===
fig, ax = plt.subplots(figsize=(18, 6))
threshold = 0.8  # Highlight areas worse than this

for i, key in enumerate(heap_data.keys()):
    df = heap_data[key]
    # Compute fragmentation ratio: 1 - (largest / free)
    frag = 1.0 - (df['largest_block'] / df['free_heap'].replace(0, 1))
    ax.plot(df['timestamp'], frag, label=labels[i], color=colors[i], linewidth=1.5)

    # Highlight high fragmentation zones
    high_frag = frag > threshold
    start = None
    for j in range(1, len(high_frag)):
        if high_frag.iloc[j] and not high_frag.iloc[j - 1]:
            start = df['timestamp'].iloc[j]
        elif not high_frag.iloc[j] and high_frag.iloc[j - 1] and start is not None:
            end = df['timestamp'].iloc[j]
            ax.axvspan(start, end, color='red', alpha=0.2)
            start = None
    if high_frag.iloc[-1] and start is not None:
        ax.axvspan(start, df['timestamp'].iloc[-1], color='red', alpha=0.2)

ax.axhline(y=threshold, color='gray', linestyle='--', linewidth=1, label='Frag. Threshold')
ax.set_title('Fragmentation Over Time (1 - largest / free)')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Fragmentation')
ax.legend()
ax.grid(True)
fig.tight_layout()
fig.savefig(RESULTS_DIR / 'fragmentation_all_configs-vector-60min-noresize-s3.png')
plt.close(fig)
print("✅ Saved: fragmentation_all_configs.png")
