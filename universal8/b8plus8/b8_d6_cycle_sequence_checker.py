from pathlib import Path
from collections import Counter,defaultdict

# read cases
def read_cases(path):
 L=open(path).read().splitlines();out=[];i=1
 while L[i]!='END':
  h=L[i].split(); B,E,core,sp=int(h[1]),int(h[2]),h[3],int(h[4]);ed=[tuple(map(int,x.split(','))) for x in L[i+1].split()[1:]];out.append((B,E,core,sp,ed));i+=3
 return out

def path_exact(adj,s,t,L):
 def dfs(v,d,used):
  if d==L:return v==t
  for w in adj[v]:
   if w in used:continue
   if w==t and d+1<L:continue
   if dfs(w,d+1,used|{w}):return True
  return False
 return dfs(s,0,{s})

def closed6(D):
 n=len(D);out=[]
 for s in range(n):
  if not D[s]:continue
  seq=[s]
  def rec():
   if len(seq)==6:
    if seq[0] in D[seq[-1]]:out.append(tuple(seq))
    return
   for w in D[seq[-1]]:
    seq.append(w);rec();seq.pop()
  rec()
 return out

def cycle_exists_len(adj,L):
 n=len(adj)
 for s in range(n):
  for f in adj[s]:
   if f<=s: # don't require, just symmetry weird; use start smallest in cycle optimization
    pass
  # require s be minimum vertex of cycle to avoid duplicates
  def dfs(v,depth,used):
   if depth==L:
    return s in adj[v]
   for w in adj[v]:
    if w==s: continue
    if w in used: continue
    if w<s: continue
    used.add(w)
    if dfs(w,depth+1,used):return True
    used.remove(w)
   return False
  for f in adj[s]:
   if f<=s:continue
   if dfs(f,2,{s,f}):return True
 return False

def local_cycle_safe(B,ed,labels):
 EP=len(ed); m=len(labels); base=B+EP; n=base+m
 adj=[set() for _ in range(n)]
 for e,(u,v) in enumerate(ed):
  x=B+e;adj[x].add(u);adj[u].add(x);adj[x].add(v);adj[v].add(x)
 # internal cycle nodes base+i, with B label and F cycle edges
 for i,lab in enumerate(labels):
  a=base+i;adj[a].add(lab);adj[lab].add(a)
 for i in range(m):
  a=base+i;b=base+(i+1)%m;adj[a].add(b);adj[b].add(a)
 return not any(cycle_exists_len(adj,L) for L in (4,8,16))

cases=read_cases(str(Path(__file__).resolve().with_name('b8_d6_cases.txt')))
stats=defaultdict(lambda:[0,0,0])
examples=[]
for idx,(B,E,core,sp,ed) in enumerate(cases):
 adj=[set() for _ in range(B)];deg=[0]*B
 for u,v in ed:adj[u].add(v);adj[v].add(u);deg[u]+=1;deg[v]+=1
 t=[4+(1 if E and v==sp else 0)-deg[v] for v in range(B)]
 U=[v for v in range(B) if t[v]>0]
 D=[set() for _ in range(B)]
 for ii,p in enumerate(U):
  for q in U[ii+1:]:
   if not path_exact(adj,p,q,2) and not path_exact(adj,p,q,6):D[p].add(q);D[q].add(p)
 inc=sum(t[v] for v in range(B) if D[v])
 stats[(B,E)][0]+=1
 if inc<12:continue
 stats[(B,E)][1]+=1
 W=closed6(D)
 # unique count vectors to reduce
 wc=[]
 seen=set()
 for w in W:
  c=tuple(Counter(w).get(v,0) for v in range(B))
  key=(w,c)
  # keep sequences since interleaving can matter full cycles
  wc.append((w,c))
 found=False
 necessary=0
 for wa,ca in wc:
  for wb,cb in wc:
   if any(ca[v]+cb[v]>t[v] for v in range(B)):continue
   necessary+=1
   seq=[]
   for k in range(6):seq.extend([wa[k],wb[k]])
   if local_cycle_safe(B,ed,seq):
    found=True;examples.append((idx,E,core,sp,t,wa,wb,seq));break
  if found:break
 if found:stats[(B,E)][2]+=1
 print('case',idx,core,sp,'inc',inc,'walks',len(W),'pairs',necessary,'LOCAL_SAFE_C12',found)
print('STATS',dict(stats))
print('EXAMPLES',len(examples))
for x in examples[:20]:print(x)
