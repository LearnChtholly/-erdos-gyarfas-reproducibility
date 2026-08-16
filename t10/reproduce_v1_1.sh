#!/usr/bin/env bash
set -euo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
cd "$HERE"

echo '[1/6] fixed-defect reconstruction / closure checks'
python3 eg_fixed_defect_closure_checker.py

echo '[2/6] cycle-component checker'
python3 eg_T10_cycle_checker.py

echo '[3/6] regenerate dangerous cases'
python3 generate_endpoint_cases.py eg_suppression_core_data.json endpoint_cases_regenerated.txt
cmp endpoint_cases.txt endpoint_cases_regenerated.txt

echo '[4/6] compile budget verifiers'
g++ -O3 -std=c++17 endpoint_budget_checker.cpp -o endpoint_budget_checker
g++ -O3 -std=c++17 endpoint_budget_verifier_direct_cycles.cpp -o endpoint_budget_verifier_direct_cycles

echo '[5/6] first endpoint-budget verifier'
./endpoint_budget_checker endpoint_cases.txt endpoint_budget_certificate.csv

echo '[6/6] independent direct-cycle endpoint-budget verifier'
./endpoint_budget_verifier_direct_cycles endpoint_cases.txt endpoint_budget_certificate.csv

echo 'PASS EG-T10-proof-v1.1'
