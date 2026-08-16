# EG Universal Additive +8 Candidate Audit v0.2

Status: **strong computer-assisted theorem candidate; not externally reviewed**.

Candidate for a minimal counterexample \(G\):
\[
|V_3(G)|\ge2|V_{\ge4}(G)|+8.
\]

Equivalently:
\[
|V_3(G)|\ge\left\lceil\frac{2|V(G)|+8}{3}\right\rceil.
\]

This does **not** solve the Erdős–Gyárfás conjecture.

## Reproduction

Requirements:
- Python 3
- NetworkX
- a C++17 compiler

Run:
```bash
./reproduce_small_b.sh
```

This independently regenerates all b=4..7 d=5/d=6 cases from NetworkX's
complete graph atlas, recompiles both global search implementations, reruns
all finite cases, and reruns both d=4,z=2 topological enumerations.

In this cleaned repository, the range b>=8 is supplied as sibling modules:
`../b8plus8/` for b=8 and `../p8/` for b>=9.

The range b<=3 uses the established published computer-search lower bound
that every counterexample has at least 17 vertices (see the proof audit note).

## External-review priorities

1. Suppression identity and parity \(d\equiv z\pmod2\).
2. Completeness of the d=4/d=5/d=6 branch reduction for b=4..7.
3. Completeness of the actual-component topology:
   - z=0: paths/cycles;
   - z=1: tripod/lollipop plus paths/cycles.
4. Correctness of both global forbidden-cycle oracles.
5. The d=4,z=2 half-edge enumeration, especially topological loops.
6. The use and attribution of the 17-vertex external lower bound.
7. Priority/novelty relative to 2026 cubic-density work.

Additional v0.2 audit file: `COMPONENT_COMPLETENESS_AUDIT.md`.
