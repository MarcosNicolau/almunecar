#!/bin/bash

FILE=$1
RESULTS=$(cat $FILE)

# Initialize the Markdown table header
echo "|Library| Benchmark Name | Iterations | Total Execution Time | Average Execution Time |"
echo "|-------|----------------|------------|----------------------|------------------------|"

current_library=""
while IFS= read -r line; do
    if [[ "$line" =~ Benchmark\ suite\ libs/([^/]+)/benchmarks ]]; then
        current_library="${BASH_REMATCH[1]}"
    elif [[ "$line" =~ ^\=\=\=\=\=\=\=\=+\ (.*)\ \(([0-9]+)\ iterations\)\ =\=\=\=\=\=\=\=\=+ ]]; then
        benchmark_name="${BASH_REMATCH[1]}"
        iterations="${BASH_REMATCH[2]}"
        benchmark_name=$(echo "$benchmark_name" | sed 's/^benchmark_//')
    elif [[ "$line" =~ Total\ execution\ took:\ ([0-9.]+)\ seconds ]]; then
        total_time="${BASH_REMATCH[1]}"
    elif [[ "$line" =~ Average\ execution\ took:\ ([0-9.]+)\ seconds ]]; then
        avg_time="${BASH_REMATCH[1]}"
        echo "| $current_library | $benchmark_name | $iterations | $total_time seconds | $avg_time seconds |"
    fi
done <<< "$RESULTS"

