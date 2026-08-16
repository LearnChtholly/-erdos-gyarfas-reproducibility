# EG B8+8 candidate audit package v0.2

Candidate extension (not yet an externally reviewed theorem):

    b >= 8  ==>  a >= 2b + 8

for a counterexample to the Erdős–Gyárfás conjecture chosen first with minimum order and then minimum size, where
`a=|V_3(G)|` and `b=|V_{>=4}(G)|`.

This is a **self-contained research/audit package**, not a publication release. The already-audited `b>=9` dependency is supplied in the sibling directory `../p8/`; the proof-critical content in this directory treats `b=8`.

## Proof split at b=8

Assuming `a < 2b+8`, with suppression defect `d=2b-e(Q)`, the only possibilities are:

- `d=5, z=1, E in {0,1}` using the two `(8,11)` cores;
- `d=6, z=0, E in {0,1}` using the 26 `(8,10)` cores.

The `d=5` branch is eliminated by the pair-cover / endpoint-skeleton tripod obstruction. The `d=6` branch is path-only after a closed-walk capacity certificate, and its paths are eliminated by a static label+endpoint charging certificate.

## Proof-critical files

- `B8PLUS8_CANDIDATE_NOTE.md` — human proof skeleton.
- `eg_fixed_defect_reps.json`, `eg_fixed_defect_closure_checker.py` — independent complete core reconstruction.
- `generate_b8_d6_cases.py`, `b8_d6_cases.txt` — generation of the 184 labelled `d=6` cases from frozen cores.
- `b8_d6_cycle_closedwalk_dp.py`, `b8_d6_cycle_sequence_checker.py` — two cycle-component checks.
- `b8_d6_label_endpoint_budget.csv`, `b8_d6_label_endpoint_budget_checker.cpp` — proof-critical static label+endpoint charging certificate and direct simple-cycle verifier.
- `b8_d5_networkx_checker.py`, `check_b8_d5_tripod_alt.cpp` — independent verification of the `d=5` branch.

## Supplementary cross-checks

- `b8_d6_exact_quota_direct.cpp`, `b8_d6_exact_quota_alt.cpp` — two exact global-quota relaxation checkers; not needed by the short charging proof.
- `../p8/` — sibling dependency module for `b>=9`.
- `B7_METHOD_BOUNDARY_EXAMPLE.py`, `b7_d6_cases.txt` — exploratory method-boundary witness at `b=7`; not used in the candidate theorem proof.

## Expected numerical outputs

- complete `(8,10)` core count: 26; complete `(8,11)` core count: 2;
- `d=6` labelled cases: `(8,0)=20`, `(8,1)=164`, total 184;
- `d=6` cycle check: no capacity-feasible `C12` pattern in any of 184 cases;
- label+endpoint charging certificate: `184/184 PASS`;
- supplementary exact-quota cross-checks: `184/184 UNSAT`, `23,495` deduplicated local count-vector options, `5,689,003` memoized states;
- `d=5`: every B-pair is covered by a Q-path of length 2 or 6; the independent tripod enumerator finds no admissible two-y tripod in either `(8,11)` core.

## Reproduction

Requirements:

- Python 3
- NetworkX
- a C++17 compiler (`g++` tested)

Run the proof-critical checks from the package directory:

    bash reproduce_quick.sh

The script uses only files inside this directory. No `/mnt/data` or other absolute path is required.

For the slower supplementary exact-quota checks:

    bash reproduce_supplementary.sh

## Status

Internal status: **strong computer-assisted theorem candidate**. No mathematical gap is currently known, but dedicated independent external/adversarial review is still required. Novelty/priority is not certified.
