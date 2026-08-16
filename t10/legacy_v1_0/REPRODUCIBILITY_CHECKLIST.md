# Reproducibility checklist - EG-T10-proof-v1.0

## Environment
- 64-bit Linux recommended.
- Python 3.10+; frozen build was assembled under Python 3.13.5.
- `networkx` 3.x; frozen build used 3.6.1.
- C++17 compiler; frozen build used `g++ 14.2.0`.
- No SAT solver, nauty, Sage, or proprietary package is required for the frozen verification commands.

## Integrity
1. Verify `SHA256SUMS.txt` from inside the extracted directory.
2. Verify the archive hash supplied alongside the release.
3. Do not mix files from v2/v3 historical packages with this directory.

## Quick verification
Run:

```bash
./reproduce_all.sh quick
```

This checks:
- suppression core/certificate consistency;
- cycle-elimination finite statistics;
- independent fixed-defect reconstruction;
- dangerous-case partition completeness;
- T14/T15 core-set cross-check;
- every static dead-state DAG certificate.

## Full independent verification
Run:

```bash
./reproduce_all.sh full
```

This additionally compiles and runs `second_local_option_verifier.cpp` over all 1943 dangerous cases. It regenerates the complete local option sets independently and reruns an independent exact-cover DP. Expected grand totals:

```text
TOTAL CASES 1943 OPTIONS 97345 TRIGGERED_CANDIDATES 31446946 DP_STATES 2119672
```

Every certificate/chunk must print `PASS`/`PASS2`; any disagreement is a failed reproduction.

## Expected finite counts
- `(13,19)`: 10 representatives.
- `(14,21)`: 0.
- `T14=(14,20)`: 852.
- `T15=(15,22)`: 168.
- `T16=(16,24)`: 0.
- Dangerous labeled cases: `94 + 1581 + 14 + 254 = 1943`.
- Independent option total: 97,345.

## Archival practice
If any proof-critical file is changed, do not overwrite v1.0. Create a new release and regenerate both file hashes and the archive hash.
