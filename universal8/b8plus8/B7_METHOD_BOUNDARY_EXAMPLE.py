from pathlib import Path
import networkx as nx
L=open(Path(__file__).resolve().with_name('b7_d6_cases.txt')).read().splitlines();cases=[];i=1
while L[i]!='END':
 h=L[i].split();B,E,core,sp=int(h[1]),int(h[2]),h[3],int(h[4]);ed=[tuple(map(int,x.split(','))) for x in L[i+1].split()[1:]];cases.append((B,E,core,sp,ed));i+=3
idx=17;B,E,core,sp,ed=cases[idx]
GQ=nx.Graph();GQ.add_nodes_from(range(B));GQ.add_edges_from(ed);deg=dict(GQ.degree())
t=[4+(1 if E and v==sp else 0)-deg[v] for v in range(B)]
U=[v for v in range(B) if t[v]>0]
def hasp(s,q,L):return any(len(p)-1==L for p in nx.all_simple_paths(GQ,s,q,cutoff=L))
D={v:set() for v in U}
for ii,p in enumerate(U):
 for q in U[ii+1:]:
  if not hasp(p,q,2) and not hasp(p,q,6):D[p].add(q);D[q].add(p)
print('case',idx,core,'t',t,'D',D)

def has_cycle_len(adj,L):
 n=len(adj)
 for s in range(n):
  def rec(v,depth,used,first):
   if depth==L-1:
    return s in adj[v] and first<v
   for w in adj[v]:
    if w==s or w in used:continue
    used.add(w)
    if rec(w,depth+1,used,first):return True
    used.remove(w)
   return False
  for f in adj[s]:
   if rec(f,1,{s,f},f):return True
 return False

def local_safe(seq):
 EP=len(ed);base=B+EP;n=base+len(seq);adj=[set() for _ in range(n)]
 def add(u,v):adj[u].add(v);adj[v].add(u)
 for e,(u,v) in enumerate(ed):
  x=B+e;add(x,u);add(x,v)
 for i,lab in enumerate(seq):add(base+i,lab)
 for i in range(len(seq)):add(base+i,base+(i+1)%len(seq))
 return not any(has_cycle_len(adj,L) for L in (4,8,16))

for m in [3,5,6,7,9,10,11,12]:
 rem=t[:];seq=[];nodes=[0]
 def dfs(pos):
  nodes[0]+=1
  if pos==m:
   for i in range(m):
    a=seq[i];b=seq[(i+2)%m]
    if a==b or b not in D.get(a,set()):return None
   if local_safe(seq):return tuple(seq)
   return None
  for lab in U:
   if rem[lab]<=0:continue
   if pos>=2:
    a=seq[pos-2]
    if lab==a or lab not in D.get(a,set()):continue
   rem[lab]-=1;seq.append(lab)
   r=dfs(pos+1)
   if r is not None:return r
   seq.pop();rem[lab]+=1
  return None
 r=dfs(0)
 print('m',m,'nodes',nodes[0],'found',r)
 if r is not None:
  print('LOCAL SAFE CYCLE',m,r);break
