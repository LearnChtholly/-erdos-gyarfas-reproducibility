#!/usr/bin/env python3
import json,itertools,collections,hashlib,time
from pathlib import Path
P=Path(__file__).resolve().with_name('eg_fixed_defect_reps.json')
D=json.load(open(P))['states']

def masks(n,edges):
 a=[0]*n
 for u,v in edges:a[u]|=1<<v;a[v]|=1<<u
 return tuple(a)
S={}
# Correct F keys n is encoded F7/F8/F9/F10
for k,arr in D.items():
 if k.startswith('F7_'): n=7
 elif k.startswith('F8_'): n=8
 elif k.startswith('F9_'): n=9
 elif k.startswith('F10_'): n=10
 elif k.startswith('F11_'): n=11
 else: continue
 S[k]=[masks(n,e) for e in arr]

def ec(a):return sum(x.bit_count() for x in a)//2

def path(a,s,t,L):
 def dfs(v,d,used):
  if d==L:return v==t
  c=a[v]&~used
  if d<L-1:c&=~(1<<t)
  while c:
   b=c&-c;c-=b;w=b.bit_length()-1
   if dfs(w,d+1,used|b):return True
  return False
 return dfs(s,0,1<<s)

def legal2(a,p,q,newn,full=True):
 ls=[2,6]
 if full:
  c=16
  while c<=newn:ls.append(c-2);c*=2
 return not any(path(a,p,q,L) for L in ls)

def ext(a,ns):
 n=len(a);b=list(a)+[0]
 for u in ns:b[u]|=1<<n;b[n]|=1<<u
 return tuple(b)

def vsig(a,v):
 n=len(a);d=[x.bit_count() for x in a]
 ns=[u for u in range(n) if (a[v]>>u)&1]
 tri=0
 for i in range(len(ns)):
  for j in range(i+1,len(ns)):tri+=((a[ns[i]]>>ns[j])&1)
 # distance counts 1..3
 dist=[-1]*n;dist[v]=0;q=[v]
 for x in q:
  if dist[x]>=3:continue
  c=a[x]
  while c:
   z=c&-c;c-=z;w=z.bit_length()-1
   if dist[w]<0:dist[w]=dist[x]+1;q.append(w)
 hist=tuple(dist.count(r) for r in range(1,4))
 return (d[v],tri,tuple(sorted(d[u] for u in ns)),hist)

def inv(a):return (len(a),ec(a),tuple(sorted(vsig(a,v) for v in range(len(a)))))

def iso(a,b):
 n=len(a)
 if len(b)!=n or ec(a)!=ec(b):return False
 fa=[vsig(a,v) for v in range(n)];fb=[vsig(b,v) for v in range(n)]
 if collections.Counter(fa)!=collections.Counter(fb):return False
 gb=collections.defaultdict(list)
 for v,s in enumerate(fb):gb[s].append(v)
 order=sorted(range(n),key=lambda u:(len(gb[fa[u]]),-a[u].bit_count()))
 mp=[-1]*n
 def rec(k,used):
  if k==n:return True
  u=order[k]
  for v in gb[fa[u]]:
   if used>>v&1:continue
   ok=True
   for h in range(k):
    u2=order[h];v2=mp[u2]
    if ((a[u]>>u2)&1)!=((b[v]>>v2)&1):ok=False;break
   if ok:
    mp[u]=v
    if rec(k+1,used|(1<<v)):return True
    mp[u]=-1
  return False
 return rec(0,0)

class Target:
 def __init__(self,arr):
  self.arr=arr;self.b=collections.defaultdict(list);self.hit=[False]*len(arr)
  for i,g in enumerate(arr):self.b[inv(g)].append(i)
 def match(self,g):
  for i in self.b.get(inv(g),[]):
   if iso(g,self.arr[i]):self.hit[i]=True;return True
  return False
 def allhit(self):return all(self.hit)

def check_transition(parents_by_m,target,newn,m,label):
 T=Target(target);raw=0
 for k in (0,1,2):
  for a in parents_by_m.get(m-k,[]):
   if k==0: cs=[()]
   elif k==1: cs=((p,) for p in range(newn-1))
   else: cs=(pq for pq in itertools.combinations(range(newn-1),2) if legal2(a,*pq,newn))
   for ns in cs:
    raw+=1;g=ext(a,ns)
    if not T.match(g):raise AssertionError((label,'unrepresented child',k,ns,ec(g)))
 if target and not T.allhit():raise AssertionError((label,'unhit reps',sum(not x for x in T.hit)))
 if not target and raw!=0:raise AssertionError((label,'expected empty',raw))
 print(label,'raw',raw,'reps',len(target),'PASS')

def check_d2(par,target,newn,label,full=True):
 T=Target(target);raw=0;checks=0
 for a in par:
  for p,q in itertools.combinations(range(newn-1),2):
   checks+=1
   if legal2(a,p,q,newn,full):
    raw+=1;g=ext(a,(p,q))
    if not T.match(g):raise AssertionError((label,'unrepresented',p,q))
 if target and not T.allhit():raise AssertionError((label,'unhit',sum(not x for x in T.hit)))
 if not target and raw!=0:raise AssertionError((label,'expected empty',raw))
 print(label,'checks',checks,'raw',raw,'reps',len(target),'PASS')

def check_T(Tprev,Eprev,target,newn,label):
 T=Target(target);raw=0
 for a in Eprev:
  for p in range(newn-1):
   raw+=1;g=ext(a,(p,))
   if not T.match(g):raise AssertionError((label,'d1 unrepresented'))
 for a in Tprev:
  for p,q in itertools.combinations(range(newn-1),2):
   if legal2(a,p,q,newn):
    raw+=1;g=ext(a,(p,q))
    if not T.match(g):raise AssertionError((label,'d2 unrepresented'))
 if target and not T.allhit():raise AssertionError((label,'unhit',sum(not x for x in T.hit)))
 print(label,'raw',raw,'reps',len(target),'PASS')


# Load non-F certificate states.
for k,arr in D.items():
    if k in S: continue
    if k in ('E12','T12'): n=12
    elif k in ('E13','T13'): n=13
    elif k in ('E14_21','T14'): n=14
    elif k=='T15': n=15
    elif k.startswith('T16'): n=16
    else: continue
    S[k]=[masks(n,e) for e in arr]

# Base: independently compare certificate F7 families to the public NetworkX atlas.
import networkx as nx
atlas=nx.graph_atlas_g()
def is2deg(a):
 n=len(a);alive=(1<<n)-1;deg=[x.bit_count() for x in a]
 for _ in range(n):
  v=next((u for u in range(n) if alive>>u&1 and deg[u]<=2),-1)
  if v<0:return False
  alive&=~(1<<v);c=a[v]&alive
  while c:b=c&-c;c-=b;deg[b.bit_length()-1]-=1
 return True
def c4free(a):
 return all((a[u]&a[v]).bit_count()<2 for u,v in itertools.combinations(range(len(a)),2))
for m in range(4,10):
 cert=S[f'F7_{m}'];T=Target(cert);cnt=0
 for G in atlas:
  if G.number_of_nodes()!=7 or G.number_of_edges()!=m:continue
  a=masks(7,list(G.edges()))
  if is2deg(a) and c4free(a):
   cnt+=1
   if not T.match(a):raise AssertionError(('F7',m,'atlas graph missing'))
 if cnt!=len(cert) or not T.allhit():raise AssertionError(('F7 mismatch',m,cnt,len(cert)))
 print('F7',m,cnt,'PASS')

for n,prefix,ms in [(8,'F8',(8,9,10,11)),(9,'F9',(10,11,12,13)),(10,'F10',(12,13,14,15))]:
 prev={}
 pp=f'F{n-1}'
 # possible m range from certificate keys
 for k,v in S.items():
  if k.startswith(pp+'_'):prev[int(k.split('_')[1])]=v
 for m in ms:check_transition(prev,S[f'{prefix}_{m}'],n,m,f'{prefix}_{m}')
prev={m:S[f'F10_{m}'] for m in (12,13,14,15)}
for m in (14,15,16):check_transition(prev,S[f'F11_{m}'],11,m,f'F11_{m}')
check_d2(S['F11_15'],S['E12'],12,'E12')
check_d2(S['E12'],S['E13'],13,'E13')
check_d2(S['E13'],S['E14_21'],14,'E14_21')
check_T(S['F11_14'],S['F11_15'],S['T12'],12,'T12')
check_T(S['T12'],S['E12'],S['T13'],13,'T13')
check_T(S['T13'],S['E13'],S['T14'],14,'T14')
check_d2(S['T14'],S['T15'],15,'T15')
check_d2(S['T15'],S['T16_48'],16,'T16_C4C8',full=False)
check_d2(S['T15'],S['T16_full'],16,'T16_full',full=True)
print('PASS ALL FIXED-DEFECT CLOSURES')
print('reps_sha256',hashlib.sha256(P.read_bytes()).hexdigest())
