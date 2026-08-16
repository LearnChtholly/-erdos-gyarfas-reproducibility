# Component-topology completeness audit

This note records the adversarial completeness check for the small-\(b\)
global component searches used in the Universal8 candidate.

## 1. Degree data inside \(F=G[A]\)

Every \(A\)-vertex has degree \(1,2,\) or \(3\) in \(F\).  We denote their
counts by \(x,y,z\).

### Case \(z=0\)

Every connected component of \(F\) has maximum degree 2 and has at least one
vertex. Hence it is exactly one of:
- a path (including the single-edge path, and allowing zero internal
  \(y\)-vertices), or
- a cycle consisting entirely of \(y\)-vertices.

The d=6 search therefore enumerates all component topologies by enumerating
endpoint-paired paths plus arbitrary ordinary cycles.

### Case \(z=1\)

Consider the unique connected component \(C\) containing the unique
degree-3 vertex. Let \(n_1\) be the number of degree-1 vertices of \(C\), and
let \(c\) be its cyclomatic number. The degree sum and
\(e(C)=v(C)-1+c\) give
\[
n_1+3=2+2c,\qquad\text{equivalently}\qquad n_1+2c=3.
\]
Thus there are only two possibilities:
- \(c=0,n_1=3\): a tripod;
- \(c=1,n_1=1\): a lollipop (one cycle through the degree-3 vertex and one
  tail ending at the unique degree-1 vertex).

All other components have maximum degree 2, hence are paths or ordinary
\(y\)-cycles.

The d=5 generalized search enumerates exactly:
- every tripod;
- every lollipop;
- every remaining endpoint-paired path;
- every ordinary \(y\)-cycle.

## 2. Endpoint completeness

Each degree-1 \(A\)-vertex corresponds bijectively to one edge of the
suppression graph \(Q\); the search represents it by an endpoint token.
Every endpoint token is used exactly once.

For z=0, the path recursion always takes the smallest unused endpoint token
and pairs it with every other unused endpoint. This is a complete enumeration
of perfect endpoint pairings.

For z=1, the special component first chooses:
- every 3-subset of endpoint tokens for a tripod; or
- every endpoint token for a lollipop.
The same complete path pairing recursion is then applied to all remaining
tokens.

## 3. Label-word completeness

Every internal degree-2 \(A\)-vertex has exactly one neighbour in \(B\);
the corresponding \(B\)-vertex is its label. The generators try every label
whose remaining capacity is positive. Repeated labels are allowed.

The total capacity vector is
\[
t_v=4+\varepsilon_v-d_Q(v).
\]
All final accepted assemblies must use this vector exactly.

## 4. Cycle symmetry reductions

An ordinary \(y\)-cycle has no distinguished starting vertex or orientation.
Thus rotations and reversal of its label word yield the same component after
renaming its \(A\)-vertices while fixing \(B\). Keeping one dihedral
representative is safe.

In a lollipop the degree-3 vertex fixes the point at which the cycle is
attached, so only reversal of the cycle direction is a symmetry. The code
uses only this reversal symmetry.

The three arms of a tripod are unordered at the degree-3 centre. Sorting the
three endpoint-token indices fixes one representative of this arm
permutation; for that fixed ordering every label word on every arm is still
enumerated.

No global canonical-order pruning between distinct components is used.

## 5. Global forbidden cycles

The local option generators first discard any single component that already
forms a forbidden cycle with the fixed suppression skeleton.

More importantly, the global recursion assembles all selected components in
one graph. Every new \(F\)-edge is checked. If it creates a simple cycle of
length 4, 8, or 16, that branch is rejected. Therefore cycles using edges from
two or more different \(F\)-components are also detected.

Two implementations are supplied:
- alternate simple paths of lengths 3,7,15 before adding the new edge;
- direct enumeration of simple cycles of lengths 4,8,16 containing the new
  edge.

For all b=4..7 finite branches both implementations return UNSAT.

## 6. Why no C32 check is needed

Under the contradiction assumption \(a<2b+8\) with \(b\le7\),
\[
|V(G)|=a+b<3b+8\le29.
\]
Hence a simple \(C_{32}\) cannot exist. Thus checking C4, C8 and C16 is
complete for all power-of-two cycle obstructions in these finite branches.

## 7. d=4,z=2 half-edge enumeration

For b=4,5 the unique d=4 suppression core has diameter 2, so no two
\(y\)-vertices may have a common \(F\)-neighbour.

After suppressing all y-vertices, the remaining topological multigraph has
the x degree-1 vertices and two degree-3 vertices. Pairing their half-edges
enumerates every topological edge. Pairing two half-edges belonging to the
same degree-3 vertex is allowed, so topological loops (lollipop-type local
structures) are included.

Pure y-cycle components are not represented by half-edges, but they are
already impossible: any cycle of length at least 3 contains two y-vertices at
F-distance 2, contradicting the diameter-2 suppression-core obstruction.

The expanded-graph checker rejects actual loops, parallel actual edges, and
any two y-vertices sharing an F-neighbour. No valid y=8 or y=9 expansion
exists for b=4 or b=5.
