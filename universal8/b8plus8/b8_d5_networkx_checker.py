from pathlib import Path
import json,itertools,networkx as nx
R=json.load(open(Path(__file__).resolve().with_name('eg_fixed_defect_reps.json')))['states']
for ci,ed in enumerate(R['F8_11']):
 B=8;EP=len(ed); Q=nx.Graph();Q.add_nodes_from(range(B));Q.add_edges_from(ed)
 # pair-cover every B pair by length 2 or 6
 bad=[]
 for p,q in itertools.combinations(range(B),2):
  ok=False
  for L in (2,6):
   if any(len(P)-1==L for P in nx.all_simple_paths(Q,p,q,cutoff=L)):
    ok=True;break
  if not ok:bad.append((p,q))
 print('core',ci,'B-pair-cover bad=',bad)
 assert not bad
 # 1-subdivision skeleton
 S=nx.Graph();S.add_nodes_from(range(B+EP))
 for e,(u,v) in enumerate(ed):S.add_edge(B+e,u);S.add_edge(B+e,v)
 def lens(e,f,targets):
  out=[]
  for L in targets:
   if any(len(P)-1==L for P in nx.all_simple_paths(S,B+e,B+f,cutoff=L)):out.append(L)
  return out
 short_safe=[]
 for e,f in itertools.combinations(range(EP),2):
  if not lens(e,f,(2,6,14)):short_safe.append((e,f))
 print(' core',ci,'short-safe',short_safe)
 if ci==1:
  assert not short_safe
 else:
  assert short_safe==[(3,4)]
  for le in range(EP):
   if le in (3,4):continue
   L3=lens(le,3,(4,12));L4=lens(le,4,(4,12))
   print('  long',le,'to3',L3,'to4',L4)
   assert L3 or L4
print('PASS B8 D5 NETWORKX STRUCTURE')
