# Candidate extension: additive +8 from b >= 8

## Status

**Strong computer-assisted theorem candidate for adversarial review.**
This package does not modify the frozen T10 v1.1 result. It extends the separately audited P8 candidate from b>=9 to b>=8.

Let G be a counterexample to the Erdős–Gyárfás conjecture chosen first with minimum order and then minimum size. Put

- A=V_3(G), B=V_{>=4}(G),
- a=|A|, b=|B|,
- E=sum_{v in B}(d_G(v)-4),
- z=#{u in A:d_{G[A]}(u)=3}.

Let Q be the suppression graph and d=2b-e(Q). The established identities are

    a = 2b + d + E + z,
    d == z (mod 2).

The candidate theorem is

> **B8+8 candidate.** If b>=8, then a>=2b+8.

For b>=9 this is the P8 candidate contained as `P8_dependency_v0.2.tar.gz`. The new work here treats b=8.

## 1. Reduction at b=8

The complete 7-vertex atlas gives q(7)=9 for simple 2-degenerate C4-free graphs. Hence any 8-vertex simple 2-degenerate C4,C8-free graph has at most 11 edges: deleting a degree-at-most-2 vertex from a hypothetical 12-edge graph would leave a 7-vertex graph with at least 10 edges. The fixed-defect reconstruction contains exactly two (8,11) cores, so q(8)=11.

Thus d>=5. If a<2b+8, then d+E+z<8 and d==z (mod 2). The only possibilities are

    d=5, z=1, E in {0,1},
    d=6, z=0, E in {0,1}.

The complete core families are

    |F8_11|=2  (d=5),
    |F8_10|=26 (d=6).

## 2. The d=5,z=1 branch

Here x=e(Q)=11 and y=2d+E=10+E.

For both (8,11) suppression cores, an independent NetworkX check verifies the following pair-cover property:

> Every two distinct B-vertices are joined in Q by a simple path of length 2 or 6.

Therefore no two y-vertices can lie at G[A]-distance 2. Equal B-labels would create C4; distinct labels combine with a length-2 or length-6 Q-path to create C8 or C16.

With exactly one degree-3 vertex in G[A], its component is either a subdivided tripod or a unicyclic lollipop; all other components are paths or cycles. The pair-cover property excludes every cycle and the lollipop case. Thus G[A] consists of one tripod and ordinary paths.

Each ordinary path contains at most two y-vertices. The tripod also contains at most two y-vertices: at most one arm can be nonempty, and that arm has length at most two in y-vertices.

Since x=11, the tripod uses three x-endpoints and the remaining eight endpoints form four paths. Hence there are five components, carrying at most 10 y-vertices total.

- If E=1, y=11, contradiction immediately.
- If E=0, y=10, equality forces all four paths and the tripod to contain exactly two y-vertices.

For E=0, the final tripod is excluded by a tiny endpoint-skeleton check independent of B-label assignments. In the 1-subdivision skeleton S(Q), the two short tripod endpoints have F-distance 2 through the unique degree-3 A-vertex. For the second (8,11) core, every endpoint pair has an S(Q)-path of length 2,6,or 14, immediately producing C4,C8,or C16. For the first core, the only short-endpoint pair avoiding these lengths is edge-token pair (3,4); for every possible third/long endpoint, an S(Q)-path of length 4 or 12 to at least one short endpoint combines with its F-distance 4 to create C8 or C16.

Two independent implementations (`b8_d5_networkx_checker.py` and `check_b8_d5_tripod_alt.cpp`) verify these finite skeleton facts; the latter also directly enumerates all capacity-feasible two-y tripod labelings and finds zero locally admissible tripods.

Thus the entire d=5 branch is impossible.

## 3. The d=6,z=0 branch: cycles

Here x=10 and y=12+E. Degree feasibility in the 26 complete (8,10) cores yields exactly

    (8,0): 20 labelled cases,
    (8,1): 164 labelled cases,

for 184 cases total.

Since z=0, G[A] is a disjoint union of paths and cycles. Define D_Q on positive-capacity B-labels as usual: pq is an edge iff Q has neither a simple p-q path of length 2 nor one of length 6.

For an internal A-cycle, the step-2 label sequence is a closed walk in D_Q. All 184 D_Q are verified bipartite, so the cycle length is divisible by 4. Since y<=13, only 4,8,12 remain; 4 and 8 are directly forbidden.

For a C12, the even and odd positions each give a length-6 closed walk in D_Q. Their combined label-count vector must be bounded coordinatewise by the true capacity vector t. Two independent implementations verify that no such pair of closed-walk count vectors exists in any of the 184 cases. Hence no cycle component exists.

## 4. The d=6,z=0 branch: paths by label+endpoint charging

After cycle elimination, every component is a path. For each labelled case, the static certificate supplies

    mu_v in {0,1,2} for v in B,
    lambda_e in {0,1,2} for endpoint tokens e in E(Q),

such that every locally admissible single path P with endpoints e,f and B-label count vector c(P) satisfies

    sum_v mu_v c_v(P) <= lambda_e + lambda_f,

while

    sum_v mu_v t_v > sum_e lambda_e.

The local path family is a relaxation: it uses only per-label capacity upper bounds and forbids C4,C8,C16 inside the full suppression endpoint skeleton plus the single path. It ignores global quota compatibility, cross-component cycles, trigger conditions and longer dyadic cycles. Therefore every true path belongs to the checked family.

Summing the local inequality over a true path decomposition gives

    sum_v mu_v t_v
      = sum_P sum_v mu_v c_v(P)
      <= sum_P (lambda_{e_P}+lambda_{f_P})
      = sum_e lambda_e,

contradicting the strict certificate inequality.

`b8_d6_label_endpoint_budget_checker.cpp` recomputes capacities and all locally admissible paths directly from the raw 184 cases and verifies every certificate without reading the optimizer or any option list. It passes all 184 cases.

As an independent supplementary check, two exact-quota implementations using different forbidden-cycle oracles both enumerate 23,495 deduplicated endpoint/count-vector options and return UNSAT for all 184 cases, with 5,689,003 memoized exact-cover states.

Thus the entire d=6 branch is impossible.

## 5. Candidate conclusion

The b=8 violation of a>=2b+8 reduces to the d=5 or d=6 branches, both excluded above. Together with the separately audited b>=9 P8 candidate, this yields the candidate structural bound

    b >= 8  ==>  a >= 2b + 8.

This is still a statement about hypothetical minimal counterexamples and does not resolve the Erdős–Gyárfás conjecture.

## Method boundary at b=7

The present method changes qualitatively at b=7. Among d=6 cases there are many non-bipartite bad-pair graphs D_Q, and a concrete case `D6_B7_0004,E=0` admits a locally admissible triangle component with B-label word `(0,3,4)` in the full suppression skeleton without producing C4,C8,C16. Thus the b=8 threshold is not merely an untested smaller order: the current cycle-elimination mechanism genuinely fails at b=7.
