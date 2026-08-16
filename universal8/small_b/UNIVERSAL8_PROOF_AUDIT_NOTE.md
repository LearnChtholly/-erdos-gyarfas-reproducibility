# Universal additive +8 candidate — proof audit note

## Status

**Strong computer-assisted theorem candidate. Not externally reviewed.**
This package does not claim priority and does not solve the Erdős–Gyárfás conjecture.

Let \(G\) be a counterexample chosen first with minimum order and then, subject
to that, minimum size. Put
\[
A=V_3(G),\quad B=V_{\ge4}(G),\quad a=|A|,\quad b=|B|.
\]

Candidate:
\[
\boxed{a\ge 2b+8.}
\]
Equivalently, with \(n=a+b\),
\[
\boxed{a\ge \left\lceil\frac{2n+8}{3}\right\rceil.}
\]

The already frozen T10 remains stronger in the range \(b\ge14\):
\[
a\ge2b+10.
\]

## 1. Suppression identities

Using Carr's minimal-counterexample structure, \(B\) is independent and every
vertex has a degree-3 neighbour.  If \(x,y,z\) count vertices of \(G[A]\) of
degrees \(1,2,3\), and
\[
E=\sum_{v\in B}(d_G(v)-4),
\]
then
\[
2x+y=4b+E,\qquad a=x+y+z.
\]
Suppress every degree-1 vertex of \(G[A]\) between its two \(B\)-neighbours,
forming a simple 2-degenerate graph \(Q\) on \(B\), with \(e(Q)=x\).
Thus
\[
a=4b+E+z-e(Q).
\]
Let
\[
d:=2b-e(Q).
\]
Then
\[
\boxed{a=2b+d+E+z},\qquad \boxed{d\equiv z\pmod2}.
\]

## 2. Large b

The independently reconstructed fixed-defect bounds in the B8 dependency give
\[
e(Q)\le2b-7\qquad(b\ge11).
\]
Hence \(d\ge7\).  By \(d\equiv z\pmod2\), either \(d\ge8\), or \(d=7\)
and \(z\ge1\).  Therefore \(d+z\ge8\), and
\[
a=2b+d+E+z\ge2b+8.
\]

The cases \(b=8,9,10\) are certified in the self-contained
`EG-B8plus8-candidate-audit-v0.2.tar.gz` dependency.

## 3. Small suppression cores, b=4,5,6,7

The complete NetworkX graph atlas independently gives, for simple
2-degenerate \(C_4\)-free graphs (and \(C_8\) is impossible at these orders),
\[
q(4)=4,\quad q(5)=6,\quad q(6)=7,\quad q(7)=9.
\]

Assume \(a<2b+8\). Then
\[
d+E+z\le7,\qquad d\equiv z\pmod2.
\]

For \(b=6,7\), \(d\ge5\), so the only possibilities are
\[
(d,z)=(5,1)\quad\text{or}\quad(6,0),\qquad E\in\{0,1\}.
\]

For \(b=4,5\), the same two branches occur, plus
\[
(d,z)=(4,0),\ E=0,1,2,3,
\]
and
\[
(d,z)=(4,2),\ E=0,1.
\]

### d=5 and d=6 branches

The atlas generator produces the complete degree-feasible labelled case counts:
\[
\begin{array}{c|cc}
b&(d,z)=(5,1)&(d,z)=(6,0)\\
\hline
4&15&10\\
5&24&30\\
6&22&71\\
7&32&114.
\end{array}
\]

For \(d=6,z=0\), \(G[A]\) consists of paths and cycles.
For \(d=5,z=1\), the component containing the unique degree-3 \(A\)-vertex is
necessarily a tripod or a lollipop; other components are paths or cycles.

The global searchers enumerate **actual label sequences** for every component,
enforce exact endpoint use and exact \(B\)-label quotas, assemble all chosen
components in the same graph, and reject every \(C_4,C_8,C_{16}\), including
cycles using more than one component.  All graphs here have fewer than 32
vertices, so no larger power-of-two cycle is possible.

Two independent cycle oracles are supplied:
1. alternate simple paths of lengths \(3,7,15\);
2. direct simple-cycle enumeration.

Both return UNSAT for every listed case.

### d=4, z=0

For both \(b=4,5\), the unique extremal \(Q\) has diameter \(2\); hence every
pair of distinct \(B\)-labels has a length-2 \(Q\)-path. Consequently two
\(y\)-vertices cannot occur at \(G[A]\)-distance \(2\), otherwise a \(C_4\)
(same label) or \(C_8\) (distinct labels) appears.

Here \(x=e(Q)=2b-4\), so there are \(b-2\) path components after excluding
pure \(y\)-cycles by the same distance-2 obstruction. Each path has at most
two \(y\)-vertices. Hence
\[
y\le2(b-2)\le6,
\]
whereas
\[
y=2d+E=8+E\ge8.
\]
Contradiction.

### d=4, z=2

Again no two \(y\)-vertices may be at \(G[A]\)-distance \(2\), and pure
\(y\)-cycles are impossible. Suppress the \(y\)-vertices. The remaining
topological multigraph has \(x=2b-4\) degree-1 vertices and two degree-3
vertices.  The supplied half-edge checkers enumerate every pairing of the
half-edges, including pairings of two half-edges of the same degree-3 vertex
(topological loops), and every subdivision vector with total
\(y=8\) or \(9\). They reject non-simple expanded graphs and any pair of
\(y\)-vertices having a common \(G[A]\)-neighbour.

Results:
- \(b=4\): 945 half-edge pairings; valid \(y=8\): 0; valid \(y=9\): 0.
- \(b=5\): 10,395 half-edge pairings; valid \(y=8\): 0; valid \(y=9\): 0.

Thus the d=4 branches are impossible.

## 4. b <= 3

A published modern source records the established computer-search result that
**any counterexample has at least 17 vertices**, citing Markström:
A. S. Hegde, R. B. Sandeep, P. Shashank,
*Erdős–Gyárfás conjecture on graphs without long induced paths*,
arXiv:2410.22842v2 (2025), Introduction, reference [10] to K. Markström,
*Extremal graphs for some problems on cycles in graphs*.

Therefore if \(b\le3\),
\[
a=n-b\ge17-b\ge2b+8,
\]
since \(17-b-(2b+8)=9-3b\ge0\).

Combining all ranges gives the candidate
\[
\boxed{a\ge2b+8}
\]
for every minimal counterexample.

## 5. What remains

This is an internally cross-checked, computer-assisted theorem candidate.
It still needs independent external expert review and a final priority check.
