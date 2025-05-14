import json
from collections import defaultdict
from statistics import mean

# Load data
with open("results/benchmark_results.json", "r") as f:
    non_opt = json.load(f)

with open("results/benchmark_results-optimized.json", "r") as f:
    opt = json.load(f)

# Helper to extract ETL and TYPE
def parse_key(key):
    parts = key.split(",")
    etl_flag = "ETL" if "USE_ETL=1" in parts else "STD"
    type_part = next((p for p in parts if p.startswith("TYPE=")), None)
    type_val = type_part.split("=")[1].strip() if type_part else "UNKNOWN"
    return etl_flag, type_val

# Combine non-optimized and optimized into one structure
def group_by_type(data):
    grouped = defaultdict(lambda: {"ETL": {}, "STD": {}})
    for key, func_data in data.items():
        etl_flag, type_val = parse_key(key)
        for func_name, value in func_data.items():
            grouped[type_val][etl_flag][func_name] = value
    return grouped

nonopt_grouped = group_by_type(non_opt)
opt_grouped = group_by_type(opt)

# Final analysis results per TYPE

def analyze(etl_data, label):
    print(f"\n===== {label.upper()} RESULTS =====")

    for type_name in etl_data:
        std_funcs = etl_data[type_name]["STD"]
        etl_funcs = etl_data[type_name]["ETL"]
        common_funcs = set(std_funcs) & set(etl_funcs)

        improvements = []
        regressions = []

        for func in common_funcs:
            std_val = std_funcs[func]
            etl_val = etl_funcs[func]

            if std_val == 0:
                continue  # Avoid division by zero

            pct_diff = 100 * (std_val - etl_val) / std_val
            entry = {
                "function": func,
                "STD": std_val,
                "ETL": etl_val,
                "diff": std_val - etl_val,
                "pct_improvement": pct_diff
            }

            if pct_diff >= 0:
                improvements.append(entry)
            else:
                regressions.append(entry)

        print(f"\n--- TYPE: {type_name} ---")
        print(f"Matched functions: {len(common_funcs)}")
        if improvements:
            avg_improvement = mean([e['pct_improvement'] for e in improvements])
            print(f"✅ Avg % Improvement (ETL faster): {avg_improvement:.2f}%")

            print("🔥 Top 3 Best Improvements:")
            for e in sorted(improvements, key=lambda x: x['pct_improvement'], reverse=True)[:3]:
                print(f"  - {e['function']}: {e['pct_improvement']:.2f}% faster (STD: {e['STD']}, ETL: {e['ETL']})")

        if regressions:
            print(f"⚠️ ETL was *slower* than STD in {len(regressions)} cases.")
            print("🐌 Top 3 Worst Regressions:")
            for e in sorted(regressions, key=lambda x: x['pct_improvement'])[:3]:
                print(f"  - {e['function']}: {e['pct_improvement']:.2f}% slower (STD: {e['STD']}, ETL: {e['ETL']})")


"""
def analyze(etl_data, label):
    print(f"\n===== {label.upper()} RESULTS =====")

    for type_name in etl_data:
        std_funcs = etl_data[type_name]["STD"]
        etl_funcs = etl_data[type_name]["ETL"]
        common_funcs = set(std_funcs) & set(etl_funcs)

        improvements = []
        regressions = []

        for func in common_funcs:
            std_val = std_funcs[func]
            etl_val = etl_funcs[func]

            if std_val == 0:
                continue  # Avoid division by zero

            pct_diff = 100 * (std_val - etl_val) / std_val
            entry = {
                "function": func,
                "STD": std_val,
                "ETL": etl_val,
                "diff": std_val - etl_val,
                "pct_improvement": pct_diff
            }

            if pct_diff >= 0:
                improvements.append(entry)
            else:
                regressions.append(entry)

        print(f"\n--- TYPE: {type_name} ---")
        print(f"Matched functions: {len(common_funcs)}")
        if improvements:
            print(f"Avg % Improvement: {mean([e['pct_improvement'] for e in improvements]):.2f}%")
        else:
            print("No improvements.")

        if regressions:
            print(f"⚠️ ETL was *slower* than STD in {len(regressions)} cases.")
            print("Worst regressions:")
            for e in sorted(regressions, key=lambda x: x['pct_improvement'])[:3]:
                print(f"  - {e['function']}: {e['pct_improvement']:.2f}% slower")
"""
# Analyze both non-optimized and optimized
analyze(nonopt_grouped, "non-optimized")
analyze(opt_grouped, "optimized")
