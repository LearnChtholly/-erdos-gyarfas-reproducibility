# Audit status

Current internal status: **strong computer-assisted theorem candidate; dedicated external/adversarial review still required.**

Internally rechecked:

- algebraic reduction using d=2b-e(Q) and parity d == z mod 2;
- fixed-defect core completeness at (9,12) and (10,14);
- 280 labelled degree-feasible cases regenerated with unique keys and identical edge lists;
- cycle elimination by two path-enumeration implementations;
- path elimination by two local-cycle implementations plus exact maximum-weight matching;
- an independent direct-cycle verifier of a static integer endpoint-budget certificate.

Known correction from exploration:

- the tempting layered bound `y <= sum_r nu(H_r)` is true, but for d=6 sector (9,1) the upper bound can equal y (13), so it does not by itself eliminate every case. Do not use it as the finite contradiction. Exact weighted matching remains strictly below y.

Novelty status: targeted public search has not found the exact conditional bound b>=9 => a>=2b+8, but this is **not** a novelty/priority certification.
