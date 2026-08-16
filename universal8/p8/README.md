# EG-P8 candidate audit package v0.2

This package supports adversarial review of the research candidate

    b >= 9  ==>  a >= 2b + 8

for vertex-minimal, then edge-minimal counterexamples to the Erdős–Gyárfás conjecture.

It is **not** a frozen publication release and does not modify the frozen T10 v1.1 package.

## Reproduction

Python 3, NetworkX, and a C++17 compiler are sufficient.

### 1. Rebuild d=6 cases from independently certified fixed-defect cores

```bash
python generate_d6_cases.py eg_fixed_defect_reps.json regenerated_cases.txt
```

Expected counts:

    (9,0)=22, (9,1)=206, (10,0)=4, (10,1)=48, total=280.

### 2. Cycle-elimination checks

```bash
python d6_cycle_checker.py regenerated_cases.txt
python d6_cycle_checker_networkx.py regenerated_cases.txt
```

Expected maximum incident quotas:

    7, 9, 0, 0

for sectors (9,0),(9,1),(10,0),(10,1), respectively.

### 3. Direct-cycle weighted-matching checker

```bash
g++ -O2 -std=c++17 research_d6_direct_matching.cpp -o direct_match
./direct_match regenerated_cases.txt 0 280
```

Expected maxima:

    10, 11, 2, 4

against required y values 12,13,12,13.

### 4. Independent alternate-path matching implementation

```bash
g++ -O2 -std=c++17 research_d6_alternate_matching.cpp -o alt_match
./alt_match regenerated_cases.txt 0 280
```

The `max_weight_matching` fields should again be 10,11,2,4. Note: the separate `max_matchlayers` statistic is exploratory and is **not** used in the proof; in sector (9,1) it can reach y.

### 5. Static endpoint-budget certificate

```bash
g++ -O2 -std=c++17 d6_endpoint_budget_verifier_direct_cycles.cpp -o budget_check
./budget_check regenerated_cases.txt d6_endpoint_budget_certificate.csv 0 280
```

Expected maximum budget sums:

    11, 12, 2, 4

against y=12,13,12,13.

## Important proof boundary

The weighted-layer inequality `sum_r nu(H_r)` was explored but is **not** valid as a strict obstruction for every d=6 case: one (9,1) case reaches 13. The proof uses the exact maximum-weight matching obstruction, or equivalently the independently checked integer endpoint-budget certificate.


See `INDEPENDENT_AUDIT_REPORT.md` for the fresh adversarial reproduction and the v0.1 path-fix note.
