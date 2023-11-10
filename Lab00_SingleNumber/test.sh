#!/bin/bash
i=0
path=$(pwd)

# Generate test results
for input in $path/Inputs/*; do
	((i=i+1))
	#echo "$input"
	./Lab1 "$input" >"results_$i.txt"
done

# Test against expected outputs
i=0
for output in $path/Outputs/*; do
	((i=i+1))
	#echo "$output"
	python3 GradingScript.py "$output" "results_$i.txt"
done

# Delete results
i=0
for results in $path/results_*.txt; do
	((i=i+1))
	rm "$results"
done
