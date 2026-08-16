# P8 candidate: an additive +8 bound from b >= 9

## Status

**Research candidate for adversarial external review. Not yet merged into the frozen T10 v1.1 note.**

Let G be a counterexample to the Erdős–Gyárfás conjecture, chosen first with minimum order and then, subject to that, minimum size. Put

- A = V_3(G), B = V_{>=4}(G),
- a = |A|, b = |B|,
- E = sum_{v in B}(d_G(v)-4),
- z = #{u in A : d_{G[A]}(u)=3}.

Suppress every A-vertex having G[A]-degree 1 to obtain a simple 2-degenerate graph Q on B. Let

    d = 2b - e(Q).

The established suppression identity and parity relation give

    a = 2b + d + E + z,
    d == z (mod 2).

The candidate theorem is

> **P8 candidate.** If b >= 9, then a >= 2b + 8.

This is a structural statement about hypothetical minimal counterexamples. It does not prove the Erdős–Gyárfás conjecture.

## 1. Orders b >= 11

The independently certified fixed-defect extremal bound gives

    e(Q) <= 2b - 7  for b >= 11,

hence d >= 7. Since d == z (mod 2) and z >= 0:

- if d is odd, then z is odd, so z >= 1 and d+z >= 8;
- if d is even, then d >= 8, so d+z >= 8.

Therefore

    a = 2b + d + E + z >= 2b + 8

for every b >= 11.

## 2. Reduction for b = 9,10

Exact fixed-defect data give

    q(9)=12,   q(10)=14,

so d >= 6 for b=9,10.

Assume for contradiction that a < 2b+8. Then

    d + E + z < 8,
    d >= 6,
    d == z (mod 2).

The only possibility is

    d=6, z=0, E in {0,1}.

Indeed d=7 forces z>=1, while d>=8 is already impossible.

Thus only the d=6 extremal suppression cores remain:

- b=9, e(Q)=12: 33 non-isomorphic cores;
- b=10, e(Q)=14: 14 non-isomorphic cores.

For z=0,

    y = #{u in A : d_{G[A]}(u)=2} = 2d+E = 12+E.

For a vertex v in B define its internal-label capacity

    t_v = 4 + epsilon_v - d_Q(v),

where epsilon_v=0 for E=0, while for E=1 exactly one distinguished vertex w has epsilon_w=1.

Degree feasibility produces exactly

    (b,E)=(9,0):   22 cases,
           (9,1):  206 cases,
           (10,0): 4 cases,
           (10,1): 48 cases,

for a total of 280 labelled cases.

## 3. Elimination of cycle components

Because z=0 and every A-vertex has an A-neighbour, G[A] is a disjoint union of paths and cycles.

For positive-capacity B-labels p,q define the bad-pair graph D_Q by putting pq in E(D_Q) iff Q contains neither

- a simple p-q path of length 2, nor
- a simple p-q path of length 6.

If labels p,q occur on a G[A]-cycle at F-distance 2, then:

- p=q creates a C4 in G;
- a length-2 Q-path creates a C8 after lifting suppression edges;
- a length-6 Q-path creates a C16.

Hence the step-2 label sequence of every internal cycle is a closed walk in D_Q. In every one of the 280 cases, two independent implementations verify that D_Q is a forest (hence bipartite). Therefore the cycle length m must satisfy 4 | m.

Since y<=13, only m=4,8,12 are possible. The first two are forbidden directly. For m=12 all twelve label occurrences must use vertices incident with D_Q. The verified maximum total incident capacity is

    (9,0): 7,
    (9,1): 9,
    (10,0): 0,
    (10,1): 0,

all strictly below 12. Thus no cycle component exists.

Therefore G[A] is path-only in every remaining d=6 case.

## 4. Path elimination: weighted endpoint matching

Every edge of Q corresponds to one G[A]-degree-1 endpoint token. For distinct endpoint tokens e,f let m_ef be the maximum number of internal y-vertices in a **locally admissible single path** joining e to f, where the relaxed local model imposes only conditions necessary for every true path:

1. usage of each B-label v is at most t_v;
2. the local graph consisting of the full suppression endpoint skeleton plus this one path contains no C4, C8, or C16;
3. repeated B-labels are allowed;
4. global quota compatibility between different paths, cross-component forbidden cycles, component-trigger conditions, and longer power-of-two cycles are deliberately ignored.

Thus every true path is included in this relaxed family.

The endpoint pairs of a true path decomposition form a matching M. If r(P) is the number of internal vertices in path P with endpoint pair ef, then r(P)<=m_ef. Consequently

    y = sum_P r(P)
      <= sum_{ef in M} m_ef
      <= nu_w,

where nu_w is the maximum weight of a matching in the endpoint compatibility graph with edge weights m_ef.

Two independent local-cycle implementations, both combined with an exact subset-DP maximum-weight matching computation, give the following sector maxima:

| (b,E) | cases | max nu_w | required y |
|---|---:|---:|---:|
| (9,0) | 22 | 10 | 12 |
| (9,1) | 206 | 11 | 13 |
| (10,0) | 4 | 2 | 12 |
| (10,1) | 48 | 4 | 13 |

Hence every remaining d=6 path-only case is impossible.

## 5. Integer charging certificate

For simpler proof checking, a static integer budget certificate was also generated. For every labelled case it gives nonnegative endpoint weights lambda_e in {0,1,2} satisfying

    m_ef <= lambda_e + lambda_f

for every locally admissible endpoint pair, while

    sum_e lambda_e < y.

A separate direct-simple-cycle verifier recomputes all locally admissible single paths from the raw case and checks these inequalities. The largest certificate sums are

| (b,E) | max sum lambda | y |
|---|---:|---:|
| (9,0) | 11 | 12 |
| (9,1) | 12 | 13 |
| (10,0) | 2 | 12 |
| (10,1) | 4 | 13 |

Thus the path contradiction can be certified without trusting the maximum-weight matching implementation.

## 6. Candidate deduction

Orders b>=11 were handled by the defect-7 extremal bound and parity. Orders b=9,10 reduce to the 280 d=6 cases, whose cycle and path components have both been excluded. Therefore the current candidate conclusion is

    b >= 9  ==>  a >= 2b + 8.

This result should remain labelled a **candidate** until a dedicated independent adversarial review checks the reduction, finite core completeness, cycle elimination and relaxed single-path certification.
