# Audit status — B8+8 candidate v0.2

Internal status: **strong computer-assisted theorem candidate**.

v0.2 changes no mathematical data from v0.1. It fixes the reproducibility engineering issue in v0.1 by removing absolute `/mnt/data/...` paths and adding self-contained reproduction scripts.

Internally checked:

- `q(7)=9` and complete reconstruction of the two `(8,11)` and 26 `(8,10)` cores;
- reduction of a hypothetical `b=8, a<2b+8` counterexample to the `d=5,z=1` or `d=6,z=0` branches with `E in {0,1}`;
- exact generation of 184 labelled `d=6` cases;
- `d=6` cycle elimination by two different closed-walk/capacity implementations;
- `d=6` path elimination by a static label+endpoint charging certificate with all weights in `{0,1,2}`;
- direct simple-cycle verification of that certificate for all 184 cases;
- supplementary exact-quota UNSAT for all 184 cases by two implementations using different local forbidden-cycle algorithms;
- `d=5` pair-cover and endpoint-skeleton tripod obstruction verified using NetworkX and an independent alternate-path C++ implementation;
- a concrete locally admissible `C3` at `b=7` documenting that the simpler cycle-elimination mechanism does not automatically extend below `b=8`.

No mathematical gap is currently known. External adversarial/domain review remains required before freezing or publication use.
