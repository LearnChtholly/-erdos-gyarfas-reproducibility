# Frozen theorem statement - T10 v1.1

Let G be a counterexample to the Erdos-Gyarfas conjecture chosen first with minimum order and, subject to that, minimum size. Put

- A = V_3(G), a = |A|;
- B = V_{>=4}(G), b = |B|.

Then

    b >= 14  ==>  a >= 2b + 10.

Equivalently,

    |V_3(G)| >= ceil((2|V(G)|+10)/3)

whenever |V_{>=4}(G)| >= 14.

This is a structural theorem about hypothetical minimal counterexamples. It does not prove the Erdos-Gyarfas conjecture. This v1.1 changes only the finite path-elimination proof: the v1.0 exact-cover/DAG argument is replaced in the main proof by independently verified integer endpoint-budget certificates. The theorem statement is unchanged.
