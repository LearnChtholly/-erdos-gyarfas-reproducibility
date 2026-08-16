#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
mode="${1:-quick}"
if [[ "$mode" != quick && "$mode" != full ]]; then echo "usage: $0 [quick|full]" >&2; exit 2; fi
python3 -c 'import networkx,sys; print("Python",sys.version.split()[0],"NetworkX",networkx.__version__)'
mkdir -p build
python3 eg_suppression_certificate_checker.py
python3 eg_T10_cycle_checker.py
python3 eg_fixed_defect_closure_checker.py
python3 case_partition_checker.py
python3 core_set_crosscheck.py
CXX=${CXX:-g++}
FLAGS='-O2 -std=c++17'
$CXX $FLAGS eg_b15_e0_dag_checker.cpp -o build/b15e0
$CXX $FLAGS eg_b15_e1_dag_checker.cpp -o build/b15e1
$CXX $FLAGS eg_b14_e0_dag_checker.cpp -o build/b14e0
$CXX $FLAGS eg_b14_e1_empty_dag_checker.cpp -o build/b14e1empty
$CXX $FLAGS eg_b14_e1_bad_dag_checker.cpp -o build/b14e1bad
./build/b15e0 eg_b15_e0_dead_dag.cert
./build/b15e1 eg_b15_e1_dead_dag.cert
./build/b14e0 eg_b14_e0_dead_dag.cert
for f in b14e1_empty_*.cert; do ./build/b14e1empty "$f"; done
for f in b14e1_bad_*.cert; do ./build/b14e1bad "$f"; done
if [[ "$mode" == full ]]; then
  $CXX $FLAGS second_local_option_verifier.cpp -o build/second_local_option_verifier
  mapfile -t EMPTY < <(printf '%s\n' b14e1_empty_*.cert | sort -V)
  mapfile -t BAD < <(printf '%s\n' b14e1_bad_*.cert | sort -V)
  ./build/second_local_option_verifier \
    eg_b15_e0_dead_dag.cert eg_b15_e1_dead_dag.cert eg_b14_e0_dead_dag.cert \
    "${EMPTY[@]}" "${BAD[@]}"
fi
echo "PASS EG-T10-proof-v1.0 mode=$mode"
