#!/usr/bin/env bash
set -euo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
cd "$HERE"

python3 regenerate_small_b_from_atlas.py

CXX="${CXX:-g++}"
FLAGS="-O2 -std=c++17"
$CXX $FLAGS global_d6_altpath.cpp -o d6_alt
$CXX $FLAGS global_d6_directcycles.cpp -o d6_direct
$CXX $FLAGS global_d5_altpath.cpp -o d5_alt
$CXX $FLAGS global_d5_directcycles.cpp -o d5_direct
$CXX $FLAGS b4_d4_z2_core_enum.cpp -o b4_d4_enum
$CXX $FLAGS b5_d4_z2_core_enum.cpp -o b5_d4_enum

./b4_d4_enum >/tmp/eg_b4_d4.out 2>/tmp/eg_b4_d4.err
./b5_d4_enum >/tmp/eg_b5_d4.out 2>/tmp/eg_b5_d4.err
cat /tmp/eg_b4_d4.err
cat /tmp/eg_b5_d4.err
grep -q 'v8=0.*v9=0' /tmp/eg_b4_d4.err
grep -q 'valid8=0.*valid9=0' /tmp/eg_b5_d4.err

for b in 4 5 6 7; do
  ./d5_alt "b${b}_d5_cases_atlas.txt" > "b${b}_d5_alt.log"
  ./d5_direct "b${b}_d5_cases_atlas.txt" > "b${b}_d5_direct.log"
  ./d6_alt "b${b}_d6_cases_atlas.txt" > "b${b}_d6_alt.log"
  ./d6_direct "b${b}_d6_cases_atlas.txt" > "b${b}_d6_direct.log"
done

python3 - <<'PY'
import re,glob,sys
expected={4:{5:15,6:10},5:{5:24,6:30},6:{5:22,6:71},7:{5:32,6:114}}
for b in range(4,8):
  for d in (5,6):
    for kind in ("alt","direct"):
      fn=f"b{b}_d{d}_{kind}.log"
      s=open(fn).read().splitlines()
      sat=sum(x.startswith("GLOBAL_SAT") for x in s)
      unsat=sum(x.startswith("GLOBAL_UNSAT") for x in s)
      print(fn,"SAT",sat,"UNSAT",unsat)
      assert sat==0 and unsat==expected[b][d]
print("PASS small-b dual implementation")
PY

echo "The b>=8 dependency is separately self-contained:"
echo "  EG-B8plus8-candidate-audit-v0.2.tar.gz"
echo "PASS Universal8 small-b reproduction"
