#!/usr/bin/env python3
"""Independently regenerate the b=4..7, d=5/6 suppression cases
from NetworkX's complete graph_atlas_g() (all unlabeled graphs through 7 vertices).
"""
from pathlib import Path
import networkx as nx

OUT=Path(__file__).resolve().parent

def is_2deg(G):
    H=G.copy()
    while H.number_of_nodes():
        v=min(H.nodes(), key=lambda u:H.degree(u))
        if H.degree(v)>2: return False
        H.remove_node(v)
    return True

def c4free(G):
    N={u:set(G.neighbors(u)) for u in G}
    V=list(G.nodes())
    return all(len(N[u]&N[v])<2 for i,u in enumerate(V) for v in V[i+1:])

# b<=7, hence a C8 is impossible in Q automatically.
def cores(atlas,b,d):
    m=2*b-d
    ans=[]
    for G0 in atlas:
        if len(G0)!=b or G0.number_of_edges()!=m: continue
        G=nx.convert_node_labels_to_integers(G0)
        if is_2deg(G) and c4free(G):
            ans.append(G)
    return ans

def feasible_e0(G):
    return all(G.degree(v)<=4 for v in G)

def feasible_e1(G):
    return [w for w in G if G.degree(w)<=5 and
            all(G.degree(v)<=4 for v in G if v!=w)]

atlas=nx.graph_atlas_g()
expected={(4,5):(3,3,12),(4,6):(2,2,8),
          (5,5):(4,4,20),(5,6):(5,5,25),
          (6,5):(4,3,19),(6,6):(11,10,61),
          (7,5):(5,4,28),(7,6):(17,14,100)}
for b in range(4,8):
  for d in (5,6):
    fam=cores(atlas,b,d)
    lines=["ENDPOINT_CASES_V1"]; n0=n1=0
    for idx,G in enumerate(fam):
      cid=f"D{d}_B{b}_A{idx:04d}"
      es=" ".join(f"{min(u,v)},{max(u,v)}" for u,v in sorted(G.edges()))
      if feasible_e0(G):
        lines += [f"CASE {b} 0 {cid} -1",f"EDGES {es}","ENDCASE"]; n0+=1
      for w in feasible_e1(G):
        lines += [f"CASE {b} 1 {cid} {w}",f"EDGES {es}","ENDCASE"]; n1+=1
    (OUT/f"b{b}_d{d}_cases_atlas.txt").write_text("\n".join(lines)+"\n")
    got=(len(fam),n0,n1)
    print(f"b={b} d={d}: cores={got[0]} E0={got[1]} E1={got[2]} total={n0+n1}")
    assert got==expected[(b,d)], (b,d,got,expected[(b,d)])

# q(4)..q(7)
for b in range(4,8):
    qs=[]
    for G0 in atlas:
        if len(G0)==b:
            G=nx.convert_node_labels_to_integers(G0)
            if is_2deg(G) and c4free(G):
                qs.append(G.number_of_edges())
    print(f"q({b})={max(qs)}")
assert [max([G.number_of_edges() for G in
            [nx.convert_node_labels_to_integers(H) for H in atlas if len(H)==b]
            if is_2deg(G) and c4free(G)])
        for b in range(4,8)] == [4,6,7,9]
print("PASS atlas regeneration")
