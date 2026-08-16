# Status and verification boundary

As of 2026-08-16, the material in this repository is an internally cross-checked research package.
It has not yet received independent external expert verification.

Internal checks represented in the package include multiple independently written forbidden-cycle
oracles, static small-integer charging certificates, complete labelled-case generation from frozen
core data, and a legacy exact-cover/DAG cross-check for T10.

The main external-review priorities are:

1. the suppression identity/parity and the proof that the suppression graph belongs to the claimed
   hereditary class;
2. completeness of the finite defect/branch reductions;
3. completeness of component-topology enumeration at small b;
4. soundness of the relaxed local-lift interface;
5. independent clean-machine reproduction of all proof-critical scripts/verifiers;
6. literature/priority checking for equivalent additive cubic-density bounds.
