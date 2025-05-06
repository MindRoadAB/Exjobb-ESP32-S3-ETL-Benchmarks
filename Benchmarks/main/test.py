import subprocess
import serial
import time
import re
import json
import os
PROJECT_DIR = os.path.expanduser('./')

# Assuming Linux
SERIAL_PORT = '/dev/ttyUSB0'  
BAUD_RATE = 115200
READ_TIME = 20  # seconds
OUTPUT_DIR = os.path.join(PROJECT_DIR, 'results')
os.makedirs(OUTPUT_DIR, exist_ok=True)
OUTPUT_JSON = os.path.join(OUTPUT_DIR , 'benchmark_results.json')

etl_options = [0, 1]
types = ['string', 'vector', 'map', 'set', 'deque']

def run_idf(use_etl, type_):
    print(f"\n>>> Running: USE_ETL={use_etl}, TYPE={type_}")

    cmd = [
        'idf.py',
        f'-DUSE_ETL={use_etl}',
        f'-DTYPE={type_}',
        'build', 'flash'
    ]
    subprocess.run(cmd, cwd=PROJECT_DIR, check=True)

def read_serial_output():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Let ESP32 reboot
    end_time = time.time() + READ_TIME
    lines = []
    while time.time() < end_time:
        line = ser.readline().decode(errors='ignore').strip()
        if line:
            print(line)
            lines.append(line)
            if "DONE" in line:
                break
    ser.close()
    return lines

def parse_output(lines):
    pattern = re.compile(r'TEST:\s+(.*?):\s+(\d+)\s+cycles/iteration$') # Change printtf's in tests to use ESP_LOGI(TAG, RESULT) t
    # to make parsing easier...
    results = {}
    for line in lines:
        match = pattern.search(line)
        if match:
            op, cycles = match.groups()
            results[op] = int(cycles)
    return results

def main():
    all_results = {}

    for etl in etl_options:
        for t in types:
            run_idf(etl, t)
            serial_lines = read_serial_output()
            parsed = parse_output(serial_lines)
            key = f'USE_ETL={etl},TYPE={t}'
            all_results[key] = parsed

    with open(OUTPUT_JSON, 'w') as f:
        json.dump(all_results, f, indent=2)
    print(f"\nResults written to {OUTPUT_JSON}")

if __name__ == '__main__':
    main()
