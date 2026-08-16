# External inputs

The Universal8 candidate uses two external literature inputs.

1. **Carr 2026 structural lemmas.**
   Avery Carr, *Every Minimal Counterexample to the Erdős–Gyárfás Conjecture
   is Predominantly Cubic*, arXiv:2605.22844 (2026).
   In particular:
   - every proper subgraph of a minimal counterexample has minimum degree <=2;
   - every vertex has a degree-3 neighbour;
   - vertices of degree at least 4 form an independent set.

2. **17-vertex lower bound.**
   A. S. Hegde, R. B. Sandeep, P. Shashank,
   *Erdős–Gyárfás conjecture on graphs without long induced paths*,
   arXiv:2410.22842v2 (2025), Introduction, states that extensive computer
   searches show that any counterexample has at least 17 vertices and cites
   K. Markström, *Extremal graphs for some problems on cycles in graphs*,
   Congressus Numerantium 171 (2004), 179–192.

Only the weaker established bound n>=17 is needed:
for b<=3,
\[
a=n-b\ge17-b\ge2b+8.
\]

A 2026 public computational campaign reports stronger current finite-order
exhaustion, but that stronger input is not needed here.
