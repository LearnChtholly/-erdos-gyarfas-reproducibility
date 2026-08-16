# Expert-review summary (one-page brief)

## What is being claimed
The Erdős-Gyárfás conjecture remains open. Consider a counterexample `G` chosen first of minimum order and then of minimum size. Let

\[
a=|V_3(G)|,\qquad b=|V_{\ge4}(G)|.
\]

The frozen computer-assisted structural result is:

\[
\boxed{b\ge14\Longrightarrow a\ge2b+10.}
\]

This is **not** a solution of the conjecture; it is a restriction on a hypothetical minimal counterexample.

## Main idea
Carr's minimal-counterexample structure implies that high-degree vertices form an independent set and every vertex has a cubic neighbor. Cubic vertices with exactly one cubic neighbor are suppressed to edges of a graph `Q` on the high-degree vertices. The resulting exact identity is

\[
a=4b+E+z-e(Q),\qquad e(Q)\equiv z\pmod2,
\]

where `E` is total degree excess above four on the high-degree side and `z` counts cubic vertices with three cubic neighbors. The suppression graph is simple, 2-degenerate, and inherits the absence of power-of-two cycles under a length-doubling lift.

Certified fixed-defect extremal bounds on simple 2-degenerate `C4,C8`-free graphs give

\[
e(Q)\le2b-8\ (b\ge14),\qquad e(Q)\le2b-9\ (b\ge16).
\]

Parity sharpens these inequalities. A failure of T10 is thereby reduced to exactly four sectors:

\[
(b,E)=(14,0),(14,1),(15,0),(15,1),\qquad z=0.
\]

Cycle components of `G[V_3]` are excluded using a bad-pair graph and a bipartite closed-walk argument. The remaining path-lift problem is relaxed using only necessary conditions and is exhaustively UNSAT.

## Computer-assisted part
- Fixed-defect core counts: `T14=852`, `T15=168`, `T16=0`.
- 1,943 dangerous labeled cases, independently checked with no missing/extra/duplicate cases.
- First route: static dead-state DAG certificates: 97,345 local options, 56,371 states, 122,151 checked branches.
- Second independent route: options regenerated from core edge lists; 31,446,946 triggered candidate components checked; exactly the same 97,345 options recovered; an independent exact-cover DP visited 2,119,672 states; all four sectors UNSAT.

## Novelty status
No priority claim is made. Public predecessors include Carr's `4/7` density result, a public 2026 campaign `+1`/strict-`2/3` density result, and an indexed Ulam autoresearch `+3` density result. Targeted searching has not found T10 or an obvious equivalent; external literature expertise is requested.

## Four questions for the reviewer
1. Are the suppression identity, simplicity/2-degeneracy of `Q`, and simple-cycle lifting fully correct?
2. Is the fixed-defect bootstrapping lemma sound and are its finite bases used with the correct hereditary assumptions?
3. Does the dangerous-sector exact-cover formulation genuinely use only necessary conditions, so that `true lift => relaxed solution`?
4. Do you know an existing source containing `|V_3|-2|V_{>=4}| >= 10` (under `b>=14`) or an equivalent/stronger structural statement?
