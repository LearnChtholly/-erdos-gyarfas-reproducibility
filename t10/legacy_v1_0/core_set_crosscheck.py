#!/usr/bin/env python3
from pathlib import Path
import json, networkx as nx
ROOT=Path(__file__).resolve().parent
A=json.loads((ROOT/'eg_suppression_core_data.json').read_text())
B=json.loads((ROOT/'eg_fixed_defect_reps.json').read_text())['states']

def mk(n,es):
 g=nx.Graph();g.add_nodes_from(range(n));g.add_edges_from(map(tuple,es));return g

def wh(g): return nx.weisfeiler_lehman_graph_hash(g,iterations=4)
def verify(label,n,left,right):
 L=[mk(n,x) for x in left];R=[mk(n,x) for x in right]
 assert len(L)==len(R)
 buckets={}
 for j,g in enumerate(R):buckets.setdefault(wh(g),[]).append((j,g))
 used=set()
 for i,g in enumerate(L):
  hits=[(j,h) for j,h in buckets.get(wh(g),[]) if j not in used and nx.is_isomorphic(g,h)]
  assert len(hits)==1,(label,'non-unique/missing match',i,len(hits))
  used.add(hits[0][0])
 assert len(used)==len(R)
 # Each side itself should be an isomorphism-class representative list.
 for family,name in [(L,'core-data'),(R,'reconstruction')]:
  bb={}
  for i,g in enumerate(family):bb.setdefault(wh(g),[]).append((i,g))
  for vals in bb.values():
   for x in range(len(vals)):
    for y in range(x+1,len(vals)):
     assert not nx.is_isomorphic(vals[x][1],vals[y][1]),(label,name,'duplicate iso class',vals[x][0],vals[y][0])
 print('PASS',label,f'{len(L)}/{len(R)} isomorphism classes matched')
verify('T14',14,[r['edges'] for r in A['T14_cores']],B['T14'])
verify('T15',15,[r['edges'] for r in A['T15_descendants_with_pair_cover_certificate']],B['T15'])
