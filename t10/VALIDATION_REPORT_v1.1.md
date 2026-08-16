# T10 v1.1 endpoint-budget validation report

## Verdict

**VALID SIMPLIFICATION.** No discrepancy was found in the endpoint-budget replacement. The frozen T10 v1.0 theorem statement is unchanged and v1.0 remains preserved as an independent supplementary proof route.

## Human endpoint-budget lemma

After the dangerous sectors have been reduced to path-only cases, every suppression edge e corresponds to one x-endpoint token x_e. Every endpoint token occurs in exactly one F-path component, and every y-vertex occurs in the interior of exactly one such path. If a path P has endpoints e_P,f_P and r(P) internal vertices, and a budget certificate satisfies

    r(P) <= lambda(e_P)+lambda(f_P)

for every true path, then summing gives

    y = sum_P r(P)
      <= sum_P (lambda(e_P)+lambda(f_P))
      = sum_e lambda(e).

Thus any certificate with sum_e lambda(e) < y rules out the lift. Paths with r=0 and repeated B-labels do not affect the argument.

## Relaxation direction

For each dangerous labeled core, the verifier considers a deliberately enlarged family of single-path local lifts. It requires only:

- two distinct endpoint tokens;
- per-label use no larger than t_v = 4 + epsilon_v - d_Q(v);
- the single-path local lift contains no C4, C8 or C16.

It does **not** require the component trigger, simultaneous global quota compatibility, a global endpoint perfect matching, cross-component cycle exclusions, C32 or larger cycles, or other minimal-counterexample constraints. A true path is therefore necessarily a verifier candidate. UNSAT / budget domination in this relaxed family is sound for the true lift problem.

## Case completeness

The dangerous cases were regenerated directly from the frozen T14 (852 classes) and T15 (168 classes) core data using degree feasibility. Counts are:

- (14,0): 94
- (14,1): 1581
- (15,0): 14
- (15,1): 254
- total: 1943

Comparison against the frozen endpoint case file gave missing=0, extra=0 and edge mismatch=0. The budget certificate contains 1943 unique keys, with missing=0, extra=0 and duplicate=0 against the case set.

## First verifier

`endpoint_budget_checker.cpp` is the first verifier. Across all 1943 cases its previously frozen chunk logs total:

- states: 6,448,733
- label attempts: 67,904,751
- closure checks: 6,068,575

It obtains sector maxima for sum lambda of 13, 15, 4 and 6.

## Independent direct-cycle verifier

`endpoint_budget_verifier_direct_cycles.cpp` was written separately. It does not use the first verifier's exact-length alternate-path routine. For every newly added internal vertex it directly enumerates simple cycles of lengths 4, 8 and 16 through that vertex. It reads no old local-option list, DAG, exact-cover state, endpoint-alpha table, or lambda-search logic.

Eight disjoint ranges covered [0,1943) without gaps. All ranges passed. Aggregated search statistics were:

- prefix states: 6,448,733
- label attempts: 67,904,751
- closure checks needed for budget violations: 4,218,585

The independently recovered sector maxima were again:

| sector | max sum lambda | y |
|---|---:|---:|
| (14,0) | 13 | 16 |
| (14,1) | 15 | 17 |
| (15,0) | 4 | 16 |
| (15,1) | 6 | 17 |

Hence every dangerous case satisfies sum lambda < y.

## Near-tight cases

All 46 cases whose budget is at least the sector maximum minus one were re-run in witness mode. In particular all four (14,1) cases with sum lambda=15 were independently checked. Full Q edge lists, t-vectors, lambda-vectors and longest locally admissible path witnesses are recorded in `ENDPOINT_BUDGET_NEAR_TIGHT_REPORT.md` and `endpoint_budget_neartight_cases.jsonl`.

## Relationship to v1.0

The v1.0 proof used 97,345 local options, static DAG certificates, and an independent exact-cover DP. Those computations are no longer logically needed for the main v1.1 path elimination, but the complete frozen v1.0 archive is included under `supplementary/` as an independent cross-check.
