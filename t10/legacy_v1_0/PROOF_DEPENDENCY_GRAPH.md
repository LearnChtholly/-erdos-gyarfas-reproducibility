# Proof dependency graph

```mermaid
flowchart TD
  C[Carr 2026: proper subgraph delta <=2; cubic neighbor; B independent]
  S[Suppression construction Q]
  I[Identity a=4b+E+z-e(Q)]
  P[Parity e(Q) = z mod 2]
  Q[Q simple, 2-degenerate, power-of-two-cycle-free]
  FD[Fixed-defect computation: D8 and D9]
  BS[Bootstrapping lemma]
  SH[Parity sharpening (3),(4)]
  R[Assume a<2b+10: reduce to b=14,15; z=0; E=0,1]
  CO[Core sets T14=852, T15=168; 1943 labeled dangerous cases]
  CY[Bad-pair graph D_Q; eliminate cycle components]
  TR[Component trigger lemma]
  RL[Relaxed path-lift model: true lift => relaxed solution]
  D1[Static dead-state DAG certificates]
  D2[Independent local-option generator + independent exact-cover DP]
  UNSAT[All four dangerous sectors UNSAT]
  T[T10: b>=14 => a>=2b+10]
  C-->S; S-->I; S-->Q; I-->P
  Q-->FD; FD-->BS; BS-->SH; I-->SH; P-->SH
  SH-->R; R-->CO; CO-->CY; C-->TR; CY-->RL; TR-->RL
  CO-->D1; RL-->D1; CO-->D2; RL-->D2; D1-->UNSAT; D2-->UNSAT
  R-->T; UNSAT-->T
```

## Human/computer boundary

- **Human-readable mathematics:** Carr inputs; suppression identity; simplicity and 2-degeneracy of `Q`; cycle lifting; parity; bootstrapping; parity sharpening; dangerous-sector reduction; component trigger; the logical direction of the relaxation; bad-pair closed-walk argument.
- **Computer-assisted finite facts:** fixed-defect extinction; core lists; dangerous-case partition; finite `D_Q` statistics; local path-option enumeration; exact-cover UNSAT.
- **Redundancy:** the path-lift finite step has two independent implementations: a static DAG certificate checker and a separate local-option generator followed by an independent DP.
