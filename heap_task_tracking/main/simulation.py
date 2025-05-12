import subprocess
import serial
import re
import time
import csv
import platform
from datetime import datetime
from pathlib import Path

""" 
Script  for parsing serial output of ESP32 heap test
"""


# Config
SYSTEM = platform.system()

if SYSTEM == 'Linux':
    SERIAL_PORT = '/dev/ttyUSB0'
elif SYSTEM == 'Darwin':
    SERIAL_PORT = '/dev/cu.usbserial-110'  
else:
    raise RuntimeError(f"Unsupported OS: {SYSTEM}")

MINUTES = 60 # as in, 60 seconds
READ_DURATION = 60 * MINUTES

BAUD_RATE = 115200

OUTPUT_DIR = Path('results')
OUTPUT_DIR.mkdir(exist_ok=True)

# Heap regex
re_free = re.compile(r'Free heap size:\s+(\d+)')
re_min = re.compile(r'Minimum free heap size:\s+(\d+)')
re_block = re.compile(r'Largest free block:\s+(\d+)')
re_ratio = re.compile(r'Ratio:\s+([\d.]+)')
re_event = re.compile(r'EVENT:\s+(.+)$')

# Config combos
configs = [
    {'USE_ETL': 0, 'USE_STATIC': 0},
    {'USE_ETL': 0, 'USE_STATIC': 1},
    {'USE_ETL': 1, 'USE_STATIC': 0},
    {'USE_ETL': 1, 'USE_STATIC': 1},
]

def run_test(use_etl, use_static):
    # Build and flash
    print(f"\n🔨 Building: USE_ETL={use_etl}, USE_STATIC={use_static}")
    subprocess.run([
        'idf.py',
        f'-DUSE_ETL={use_etl}',
        f'-DUSE_STATIC={use_static}',
        'build', 'flash'
    ], check=True)

    # File output setup
    heap_csv = OUTPUT_DIR / f'heap_data_USE_ETL={use_etl}_USE_STATIC={use_static}-vector-60min-noresize.csv'
    event_csv = OUTPUT_DIR / f'heap_events_USE_ETL={use_etl}_USE_STATIC={use_static}-vector-60min-noresize.csv'

    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser, \
         open(heap_csv, 'w', newline='') as heap_file, \
         open(event_csv, 'w', newline='') as event_file:

        heap_writer = csv.writer(heap_file)
        event_writer = csv.writer(event_file)

        heap_writer.writerow(['timestamp', 'free_heap', 'largest_block', 'ratio', 'event_context'])
        event_writer.writerow(['timestamp', 'event'])

        current_event_context = 'unknown'
        current_data = {}
        start_time = time.time()

        print(f"📡 Logging for USE_ETL={use_etl}, USE_STATIC={use_static}...")

        while time.time() - start_time < READ_DURATION:
            line = ser.readline().decode(errors='ignore').strip()
            if not line:
                continue

            print(line)  # Echo to terminal

            if match := re_event.match(line):
                current_event_context = match.group(1).strip()
                ts = round(time.time() - start_time, 1) #datetime.now().isoformat()
                print(f"[EVENT] {current_event_context}")
                event_writer.writerow([ts, current_event_context])
                event_file.flush()
                continue

            if match := re_free.search(line):
                current_data['free'] = int(match.group(1))
            elif match := re_block.search(line):
                current_data['block'] = int(match.group(1))
            if match := re_ratio.search(line):
                current_data['ratio'] = float(match.group(1))

                if len(current_data) == 3:
                    ts = round(time.time() - start_time, 1) 
                    heap_writer.writerow([
                        ts,
                        current_data['free'],
                        current_data['block'],
                        current_data['ratio'],
                        current_event_context
                    ])
                    heap_file.flush()
                    current_data = {}

        print(f"\n✅ Done: USE_ETL={use_etl}, USE_STATIC={use_static}")
        print(f"   → {heap_csv.name}")
        print(f"   → {event_csv.name}")

def main():
    for config in configs:
        run_test(config['USE_ETL'], config['USE_STATIC'])

if __name__ == '__main__':
    main()
