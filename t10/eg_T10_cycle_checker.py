#!/usr/bin/env python3
import json, hashlib
from pathlib import Path

DATA=Path(__file__).resolve().with_name('eg_suppression_core_data.json')
data=json.loads(DATA.read_text())

def adj(n,edges):
    a=[[] for _ in range(n)]
    for u,v in edges:
        assert u!=v
        a[u].append(v);a[v].append(u)
    return a

def path_exact(a,s,t,L):
    used={s}
    def dfs(v,d):
        if d==L:return v==t
        for w in a[v]:
            if w in used:continue
            if w==t and d<L-1:continue
            used.add(w)
            if dfs(w,d+1):return True
            used.remove(w)
        return False
    return dfs(s,0)

def Dinfo(n,edges,cap):
    a=adj(n,edges)
    U=[v for v,c in enumerate(cap) if c>0]
    E=[]
    for i,p in enumerate(U):
        for q in U[i+1:]:
            if not(path_exact(a,p,q,2) or path_exact(a,p,q,6)):
                E.append((p,q))
    da={v:[] for v in U};inc=set()
    for p,q in E:
        da[p].append(q);da[q].append(p);inc|={p,q}
    # forest + bipartite checks
    seen=set();forest=True;color={};bip=True
    for s in U:
        if s not in seen:
            stack=[(s,-1)]
            while stack:
                v,par=stack.pop()
                if v in seen:continue
                seen.add(v)
                for w in da[v]:
                    if w==par:continue
                    if w in seen:forest=False
                    else:stack.append((w,v))
        if s not in color:
            color[s]=0;q=[s]
            for v in q:
                for w in da[v]:
                    if w not in color:color[w]=1-color[v];q.append(w)
                    elif color[w]==color[v]:bip=False
    return len(E),forest,bip,sum(cap[v] for v in inc)

def cases(records,n,E):
    out=[]
    for qi,rec in enumerate(records):
        edges=rec['edges'];deg=[0]*n
        for u,v in edges:deg[u]+=1;deg[v]+=1
        if E==0:
            if max(deg)>4:continue
            cap=[4-d for d in deg]
            out.append((qi,Dinfo(n,edges,cap)))
        else:
            for sp in range(n):
                cap=[4+(v==sp)-deg[v] for v in range(n)]
                if min(cap)<0:continue
                out.append((qi,sp,Dinfo(n,edges,cap)))
    return out

T14=data['T14_cores']
T15=data['T15_descendants_with_pair_cover_certificate']
a=cases(T14,14,0);b=cases(T14,14,1);c=cases(T15,15,0);d=cases(T15,15,1)
assert (len(a),len(b),len(c),len(d))==(94,1581,14,254)
assert all(x[1][1] and x[1][2] for x in a)
assert all(x[2][1] and x[2][2] for x in b)
assert sum(x[1][0]==0 for x in a)==62
assert sum(x[2][0]==0 for x in b)==1015
assert all(x[1][0]==0 for x in c)
assert all(x[2][0]==0 for x in d)
assert max(x[1][3] for x in a if x[1][0])==7
assert max(x[2][3] for x in b if x[2][0])==9
print('PASS')
print('b14 E0: 62 empty D_Q, 32 forest D_Q, max incident quota 7')
print('b14 E1: 1015 empty D_Q, 566 forest D_Q, max incident quota 9')
print('b15 E0: 14/14 empty D_Q')
print('b15 E1: 254/254 empty D_Q')
print('data_sha256',hashlib.sha256(DATA.read_bytes()).hexdigest())
