#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
CXX=${CXX:-g++}
CXXFLAGS=${CXXFLAGS:--O2 -std=c++17}
seq 0 183 > .all_indices.txt

echo '[supplementary 1/3] explicit sequence cycle checker'
python b8_d6_cycle_sequence_checker.py

echo '[supplementary 2/3] exact quota direct-cycle implementation'
$CXX $CXXFLAGS b8_d6_exact_quota_direct.cpp -o .exact_direct
./.exact_direct b8_d6_cases.txt > exact_quota_direct.log
rm .exact_direct

echo '[supplementary 3/3] exact quota alternate-path implementation'
$CXX $CXXFLAGS b8_d6_exact_quota_alt.cpp -o .exact_alt
./.exact_alt b8_d6_cases.txt .all_indices.txt > exact_quota_alt.log
rm .exact_alt .all_indices.txt

echo 'PASS B8+8 v0.2 supplementary reproduction'
