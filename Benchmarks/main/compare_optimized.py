import json
import os

# === HARD CODED FILE PATHS ===
PROJECT_DIR = os.path.expanduser('./results')
NON_OPTIMIZED_JSON = os.path.join(PROJECT_DIR, 'benchmark_results.json')
OPTIMIZED_JSON     = os.path.join(PROJECT_DIR, 'benchmark_results-optimized.json')
OUTPUT_JSON        = os.path.join(PROJECT_DIR, 'benchmark_comparison.json')

def load_json(path):
    with open(path, 'r') as f:
        return json.load(f)

def compare_results(nonopt_data, opt_data):
    comparison = {}

    for key in nonopt_data:
        if key not in opt_data:
            continue

        nonopt_ops = nonopt_data[key]
        opt_ops = opt_data[key]

        comparison[key] = {}

        for op, nonopt_cycles in nonopt_ops.items():
            opt_cycles = opt_ops.get(op)
            if opt_cycles is None:
                continue

            reduction = nonopt_cycles - opt_cycles
            percent = (reduction / nonopt_cycles) * 100 if nonopt_cycles != 0 else 0

            comparison[key][op] = {
                "non_optimized": nonopt_cycles,
                "optimized": opt_cycles,
                "reduction": reduction,
                "percent_improvement": round(percent, 2)
            }

    return comparison

def main():
    if not os.path.exists(NON_OPTIMIZED_JSON) or not os.path.exists(OPTIMIZED_JSON):
        raise FileNotFoundError("One or both input JSON files are missing.")

    nonopt_data = load_json(NON_OPTIMIZED_JSON)
    opt_data = load_json(OPTIMIZED_JSON)

    result = compare_results(nonopt_data, opt_data)

    with open(OUTPUT_JSON, 'w') as f:
        json.dump(result, f, indent=2)

    print(f"Comparison written to: {OUTPUT_JSON}")

if __name__ == '__main__':
    main()
