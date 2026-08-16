# Freeze verification record

During assembly of v1.0, the following checks were rerun directly on the frozen copies:

- suppression core/certificate checker: PASS;
- cycle-elimination checker: PASS with b14 E0 = 62 empty/32 forest/max quota 7, b14 E1 = 1015 empty/566 forest/max quota 9, and all b15 cases empty D_Q;
- independent fixed-defect reconstruction: PASS through `(14,21)=0`, `T14=852`, `T15=168`, `T16=0`;
- independently reconstructed dangerous-case partition: PASS, 1943 total, missing=0, extra=0, duplicates=0;
- T14/T15 core-set isomorphism cross-check: 852/852 and 168/168 PASS;
- static DAG main sectors: `(15,0)`, `(15,1)`, `(14,0)` PASS;
- all 11 b14/E1 empty-D_Q chunks PASS;
- all 10 b14/E1 nonempty-D_Q chunks PASS.

The second independent local-option implementation and its complete run are frozen as `second_local_option_verifier.cpp` and `second_full_reproduction.log`. The complete prior run recorded:

`TOTAL CASES 1943 OPTIONS 97345 TRIGGERED_CANDIDATES 31446946 DP_STATES 2119672`.

The source, certificate data and prior full-run log are byte-for-byte inherited from the audited v3 precursor; see `PROVENANCE.md` and `SHA256SUMS.txt`.
