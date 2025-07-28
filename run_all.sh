#!/bin/bash
# To run this, chmod +x run_all.sh

echo "[1/3] Building the project..."
cmake -S . -B build
cmake --build build

echo "[2/3] Running simulations in parallel..."
python3 run_batch.py

echo "[3/3] All jobs submitted. Output saved to data/"