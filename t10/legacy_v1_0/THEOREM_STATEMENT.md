# Frozen theorem statement - EG-T10 v1.0

**Scope.** All graphs are finite, simple, and undirected. Let `G` be a counterexample to the Erdős-Gyárfás conjecture, chosen first with minimum order and, subject to that, minimum size. Put

\[
A=V_3(G),\qquad B=V_{\ge4}(G),\qquad a=|A|,\qquad b=|B|.
\]

## T10

If \(b\ge14\), then

\[
\boxed{a\ge2b+10.}
\]

Equivalently,

\[
\boxed{|V_3(G)|\ge \left\lceil\frac{2|V(G)|+10}{3}\right\rceil}
\qquad\text{when }|V_{\ge4}(G)|\ge14.
\]

This is a **structural theorem about a hypothetical lexicographically minimal counterexample**. It does **not** prove the Erdős-Gyárfás conjecture and does **not** assert that a counterexample exists.

**Status of this release.** The proof chain has passed internal adversarial review and two independent computational verification paths. Priority/novelty is not claimed; see `NOVELTY_SEARCH_REPORT.md`.
