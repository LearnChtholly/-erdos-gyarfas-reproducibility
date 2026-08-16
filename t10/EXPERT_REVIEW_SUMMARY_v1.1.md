# Expert review summary - T10 v1.1

The Erdos-Gyarfas conjecture remains open. This package concerns only the structure of a hypothetical counterexample chosen first with minimum order and then minimum size.

## Statement

Let a=|V_3(G)| and b=|V_{>=4}(G)|. The candidate structural theorem is

    b >= 14  ==>  a >= 2b+10.

This is not a proof of the Erdos-Gyarfas conjecture, and priority is not claimed here.

## Proof architecture

1. Carr's minimal-counterexample structure.
2. Suppression of selected cubic vertices to a simple 2-degenerate graph Q.
3. Exact identity a = 4b+E+z-e(Q) and parity e(Q)=z mod 2.
4. Computer-assisted fixed-defect bounds D8/D9.
5. Reduction of a hypothetical T10 failure to four sectors (14,0),(14,1),(15,0),(15,1), totaling 1943 labeled cases.
6. A bad-pair closed-walk argument eliminates cycle components of G[V_3].
7. **v1.1 simplification:** a small endpoint-budget certificate eliminates every remaining path-only lift.

## Endpoint-budget certificate

For each dangerous case, every suppression endpoint e receives lambda(e) in {0,1,2}. A local-path verifier checks that every capacity-feasible single path with endpoints e,f that is C4,C8,C16-free has

    r <= lambda(e)+lambda(f),

where r is the number of internal degree-two vertices of G[V_3] on that path. The certificate also has

    sum_e lambda(e) < y,

where y is the total number of such internal vertices. Since every endpoint token is used exactly once in a path decomposition,

    y = sum_P r(P) <= sum_e lambda(e) < y,

which is impossible.

The sector maxima are 13<16, 15<17, 4<16 and 6<17.

## Independent machine checks

The 1943-case set is regenerated directly from the fixed-defect cores. Certificate keys have missing=0, extra=0, duplicate=0.

Two separately written path verifiers agree:

- the first uses an exact-length alternate-path local-cycle oracle;
- the second directly enumerates simple C4/C8/C16 cycles through each newly added internal vertex and does not read old local options, DAG states, endpoint-alpha data, or lambda-search logic.

All 1943 cases pass. All 46 max/near-max budget cases were additionally run in witness mode.

The full frozen v1.0 exact-cover/DAG proof is retained as supplementary independent verification, not as a dependency of the v1.1 budget proof.

## Four questions for an external reviewer

1. Is the suppression identity / 2-degeneracy / cycle-lifting argument sound?
2. Is the fixed-defect bootstrapping argument complete?
3. Is the endpoint-budget local family genuinely a relaxation of every true path component, so that no extra restriction has been introduced?
4. Are you aware of an existing result equivalent to or stronger than the conditional +10 bound?
