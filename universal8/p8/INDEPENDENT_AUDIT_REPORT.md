# Independent adversarial audit report — P8 v0.2

Candidate:

\[
b\ge 9 \Longrightarrow a\ge 2b+8
\]

for a vertex-minimal, then edge-minimal counterexample to the Erdős–Gyárfás conjecture.

## Verdict

**Internal adversarial status: VALID AS A COMPUTER-ASSISTED THEOREM CANDIDATE.**
External human/domain review and novelty/priority review remain outstanding.

No mathematical proof gap was found in the following chain:

1. suppression identity in defect form
   \(a=2b+d+E+z\), with \(d=2b-e(Q)\);
2. parity \(d\equiv z\pmod 2\);
3. for \(b\ge11\), the certified bound \(e(Q)\le2b-7\), hence \(d+z\ge8\);
4. for \(b=9,10\), exact bounds \(q(9)=12,q(10)=14\), reducing a violation of +8 to \(d=6,z=0,E\in\{0,1\}\);
5. regeneration of all 280 degree-feasible labelled cases;
6. cycle-component elimination using the bad-pair graph \(D_Q\);
7. path-only elimination by relaxed local single-path capacities and maximum-weight endpoint matching;
8. independent integer endpoint-budget certificates checked by a separate direct-simple-cycle verifier.

## Fresh reproduction performed in this audit

The fixed-defect closure chain was rebuilt from the public 7-vertex NetworkX graph atlas and the packaged representatives. In particular:

- \(|F_{9,12}|=33\), \(|F_{9,13}|=0\), hence \(q(9)=12\);
- \(|F_{10,14}|=14\), \(|F_{10,15}|=0\), hence \(q(10)=14\);
- \(|F_{11,15}|=245\), \(|F_{11,16}|=0\), giving the defect-7 base for \(b\ge11\).

The 280 labelled cases were regenerated exactly:

- (9,0): 22;
- (9,1): 206;
- (10,0): 4;
- (10,1): 48.

The regenerated case file was byte-identical to the packaged case file.

Two cycle checkers independently returned maximum incident quotas 7,9,0,0 and verified every bad-pair graph is a forest/bipartite.

Two path-capacity/matching implementations independently returned maximum matching weights 10,11,2,4 against required y values 12,13,12,13.

The static integer endpoint-budget certificate was independently verified for all 280 cases, with maximum budget sums 11,12,2,4.

## Engineering correction

The v0.1 copy of `eg_fixed_defect_closure_checker.py` used an absolute `/mnt/data/...` path for the representative file. This was a reproducibility/packaging defect, not a mathematical defect. v0.2 resolves the representative file relative to the checker itself.

## Remaining risks

- External expert review has not yet been performed.
- Novelty/priority is not certified.
- As with any computer-assisted proof, an implementation error shared across purportedly independent programs is logically possible; the package mitigates this with independent cycle/path implementations and a static integer charging certificate.
