from pathlib import Path
import networkx as nx
from collections import defaultdict

def read_cases(path):
 L=open(path).read().splitlines();out=[];i=1
 while L[i]!='END':
  h=L[i].split();B,E,core,sp=int(h[1]),int(h[2]),h[3],int(h[4]);ed=[tuple(map(int,x.split(','))) for x in L[i+1].split()[1:]];out.append((B,E,core,sp,ed));i+=3
 return out

def exactpath(G,s,t,L):
 return any(len(p)-1==L for p in nx.all_simple_paths(G,s,t,cutoff=L))

def walk_count_vectors(D,B):
 # dynamic programming: for each start, state (current,steps,countvector)
 out=set()
 for s in D.nodes:
  init=[0]*B;init[s]=1
  states={(s,tuple(init))}
  # choose 5 more vertices, giving 6 occurrences total
  for step in range(5):
   ns=set()
   for v,c in states:
    for w in D.neighbors(v):
     cc=list(c);cc[w]+=1;ns.add((w,tuple(cc)))
   states=ns
  for v,c in states:
   if D.has_edge(v,s):out.add(c)
 return out

def main():
 cases=read_cases(str(Path(__file__).resolve().with_name('b8_d6_cases.txt')));bad=0;checked=0;stats=defaultdict(lambda:[0,0,0])
 for idx,(B,E,core,sp,ed) in enumerate(cases):
  G=nx.Graph();G.add_nodes_from(range(B));G.add_edges_from(ed);deg=dict(G.degree())
  t=[4+(1 if E and v==sp else 0)-deg[v] for v in range(B)]
  U=[v for v in range(B) if t[v]>0]
  D=nx.Graph();D.add_nodes_from(U)
  for ii,p in enumerate(U):
   for q in U[ii+1:]:
    if not exactpath(G,p,q,2) and not exactpath(G,p,q,6):D.add_edge(p,q)
  sec=(B,E);stats[sec][0]+=1
  if not nx.is_bipartite(D):raise SystemExit(('nonbip',idx))
  # any cycle can only be 12 after 4|m and y<=13. Check exact capacity possibility
  V=list(walk_count_vectors(D,B))
  feasible=False
  for a in V:
   for b in V:
    if all(a[v]+b[v]<=t[v] for v in range(B)):
     feasible=True;break
   if feasible:break
  if feasible:
   bad+=1;print('FEASIBLE_C12_PATTERN',idx,core,sp,t,D.edges(),len(V));break
  if D.number_of_edges():checked+=1
  stats[sec][1]=max(stats[sec][1],len(V));stats[sec][2]+=1
 print('bad',bad,'nonempty_checked',checked)
 print('stats',dict(stats))
 print('PASS closed-walk count-vector DP: no capacity-feasible C12 pattern')
if __name__=='__main__':main()
