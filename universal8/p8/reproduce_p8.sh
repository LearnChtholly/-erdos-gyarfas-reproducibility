#!/usr/bin/env bash
set -euo pipefail
python generate_d6_cases.py eg_fixed_defect_reps.json regenerated_cases.txt
python d6_cycle_checker.py regenerated_cases.txt
python d6_cycle_checker_networkx.py regenerated_cases.txt
${CXX:-g++} -O2 -std=c++17 research_d6_direct_matching.cpp -o direct_match
./direct_match regenerated_cases.txt 0 280
${CXX:-g++} -O2 -std=c++17 research_d6_alternate_matching.cpp -o alt_match
./alt_match regenerated_cases.txt 0 280
${CXX:-g++} -O2 -std=c++17 d6_endpoint_budget_verifier_direct_cycles.cpp -o budget_check
./budget_check regenerated_cases.txt d6_endpoint_budget_certificate.csv 0 280
