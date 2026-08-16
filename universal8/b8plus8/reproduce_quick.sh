#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
CXX=${CXX:-g++}
CXXFLAGS=${CXXFLAGS:--O2 -std=c++17}

echo '[1/6] fixed-defect reconstruction'
python eg_fixed_defect_closure_checker.py

echo '[2/6] regenerate d=6 cases and compare'
python generate_b8_d6_cases.py eg_fixed_defect_reps.json b8_d6_cases.regen.txt
cmp b8_d6_cases.txt b8_d6_cases.regen.txt
rm b8_d6_cases.regen.txt

echo '[3/6] d=6 cycle closed-walk DP'
python b8_d6_cycle_closedwalk_dp.py

echo '[4/6] d=5 NetworkX structure'
python b8_d5_networkx_checker.py

echo '[5/6] d=5 independent alternate-path checker'
$CXX $CXXFLAGS check_b8_d5_tripod_alt.cpp -o .check_b8_d5_tripod_alt
./.check_b8_d5_tripod_alt
rm .check_b8_d5_tripod_alt

echo '[6/6] d=6 proof-critical label+endpoint budget checker'
$CXX $CXXFLAGS b8_d6_label_endpoint_budget_checker.cpp -o .b8_budget_checker
./.b8_budget_checker b8_d6_cases.txt b8_d6_label_endpoint_budget.csv
rm .b8_budget_checker

echo 'PASS B8+8 v0.2 proof-critical reproduction'
