# EG-T10-proof-v1.1

Frozen external-review package for the T10 structural theorem candidate concerning minimal counterexamples to the Erdos-Gyarfas conjecture.

## Main theorem

For a counterexample G chosen first with minimum order and then minimum size, with a=|V_3(G)| and b=|V_{>=4}(G)|,

    b >= 14  ==>  a >= 2b+10.

This does not solve the Erdos-Gyarfas conjecture.

## What changed from v1.0

The theorem statement and all earlier suppression/fixed-defect/cycle-component arguments are unchanged. The main path-only elimination is simplified to an integer endpoint-budget certificate. The large v1.0 exact-cover/DAG proof is retained under `legacy_v1_0/` as an independent cross-check; only its old manuscript PDF/TEX files were omitted to keep code and paper separate.

## Main v1.1 files

- `endpoint_budget_certificate.csv` - one lambda-vector per dangerous labeled case.
- `endpoint_budget_checker.cpp` - first budget verifier.
- `endpoint_budget_verifier_direct_cycles.cpp` - independently written direct simple-cycle verifier.
- `generate_endpoint_cases.py` - regenerates the 1943 dangerous cases from fixed core data.
- `endpoint_cases.txt` - frozen generated case set.
- `ENDPOINT_BUDGET_NEAR_TIGHT_REPORT.md` - detailed max/near-max cases.
- `VALIDATION_REPORT_v1.1.md` - adversarial validation record.
- `legacy_v1_0/` - old exact-cover/DAG proof route, with manuscript files omitted.

## Expected case counts

    (14,0):   94
    (14,1): 1581
    (15,0):   14
    (15,1):  254
    total:   1943

Expected maximum budget sums are 13, 15, 4, 6 versus y=16,17,16,17.

## Reproduction

Run:

    bash reproduce_v1_1.sh

A full run includes fixed-defect/cycle checks, case regeneration, and both endpoint-budget verifiers. Runtime depends strongly on CPU. The second verifier intentionally uses direct simple-cycle enumeration rather than the first verifier's alternate-path oracle.
