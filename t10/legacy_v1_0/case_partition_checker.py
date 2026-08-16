#!/usr/bin/env python3
from pathlib import Path
import json, glob
ROOT=Path(__file__).resolve().parent
D=json.loads((ROOT/'eg_suppression_core_data.json').read_text())

def norm_edges(es): return tuple(sorted(tuple(sorted(e)) for e in es))
def degs(n,es):
 d=[0]*n
 for u,v in es:d[u]+=1;d[v]+=1
 return d

def path_exact(n,es,s,t,L):
 a=[[] for _ in range(n)]
 for u,v in es:a[u].append(v);a[v].append(u)
 used={s}
 def rec(v,k):
  if k==L:return v==t
  for w in a[v]:
   if w in used:continue
   if w==t and k<L-1:continue
   used.add(w)
   if rec(w,k+1):return True
   used.remove(w)
  return False
 return rec(s,0)

def dq_empty(n,es,sp,E):
 d=degs(n,es); cap=[4+(1 if E and v==sp else 0)-d[v] for v in range(n)]
 U=[v for v,c in enumerate(cap) if c>0]
 for i,p in enumerate(U):
  for q in U[i+1:]:
   if not path_exact(n,es,p,q,2) and not path_exact(n,es,p,q,6): return False
 return True

def parse(path,sector):
 lines=Path(path).read_text().splitlines(); out=[]; i=1
 while i<len(lines):
  if lines[i]=='ENDCERT': break
  P=lines[i].split(); assert P[0]=='CASE'
  if sector in ('B15E0','B15E1'): sp=int(P[2])
  elif sector=='B14E0': sp=int(P[3])
  else: sp=int(P[3])
  i+=1; E=lines[i].split(); assert E[0]=='EDGES'
  es=norm_edges(tuple(map(int,x.split(','))) for x in E[1:])
  out.append((es,sp))
  while lines[i]!='ENDCASE': i+=1
  i+=1
 return out

T14=[norm_edges(r['edges']) for r in D['T14_cores']]
T15=[norm_edges(r['edges']) for r in D['T15_descendants_with_pair_cover_certificate']]

def expected(cores,n,E):
 out=[]
 for es in cores:
  d=degs(n,es)
  if E==0:
   if max(d)<=4: out.append((es,-1))
  else:
   for w in range(n):
    if d[w]<=5 and all(d[v]<=4 for v in range(n) if v!=w): out.append((es,w))
 return out

actual={}
actual['B14E0']=parse(ROOT/'eg_b14_e0_dead_dag.cert','B14E0')
actual['B15E0']=parse(ROOT/'eg_b15_e0_dead_dag.cert','B15E0')
actual['B15E1']=parse(ROOT/'eg_b15_e1_dead_dag.cert','B15E1')
empty=[];bad=[]
for p in sorted(ROOT.glob('b14e1_empty_*.cert')): empty+=parse(p,'B14E1')
for p in sorted(ROOT.glob('b14e1_bad_*.cert')): bad+=parse(p,'B14E1')
actual['B14E1']=empty+bad
exp={'B14E0':expected(T14,14,0),'B14E1':expected(T14,14,1),'B15E0':expected(T15,15,0),'B15E1':expected(T15,15,1)}
want={'B14E0':94,'B14E1':1581,'B15E0':14,'B15E1':254}
for k in want:
 A=actual[k]; X=exp[k]
 assert len(A)==want[k]==len(X),(k,len(A),len(X))
 assert len(set(A))==len(A),(k,'duplicates')
 assert set(A)==set(X),(k,'missing/extra',len(set(X)-set(A)),len(set(A)-set(X)))
 print('PASS',k,'cases',len(A),'missing 0 extra 0 duplicates 0')
assert len(empty)==1015 and len(bad)==566
assert all(dq_empty(14,es,sp,1) for es,sp in empty)
assert all(not dq_empty(14,es,sp,1) for es,sp in bad)
print('PASS B14E1 D_Q partition empty=1015 nonempty=566')
allids=[(k,)+x for k in actual for x in actual[k]]
assert len(allids)==1943==len(set(allids))
print('PASS TOTAL 1943 UNIQUE 1943')
