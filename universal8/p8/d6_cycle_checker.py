#!/usr/bin/env python3
from collections import defaultdict
import sys

def read_cases(path):
    L=open(path).read().splitlines(); out=[]; i=1
    assert L[0]=='ENDPOINT_CASES_V1'
    while L[i]!='END':
        if not L[i]: i+=1; continue
        p=L[i].split(); B,E,core,sp=int(p[1]),int(p[2]),p[3],int(p[4])
        ed=[tuple(map(int,x.split(','))) for x in L[i+1].split()[1:]]
        assert L[i+2]=='ENDCASE'; out.append((B,E,core,sp,ed)); i+=3
    return out

def path_exact(adj,s,t,L):
    def dfs(v,d,used):
        if d==L:return v==t
        for w in adj[v]:
            if w in used: continue
            if w==t and d+1<L: continue
            used.add(w)
            if dfs(w,d+1,used): return True
            used.remove(w)
        return False
    return dfs(s,0,{s})

def forest_bip(n,edges):
    adj=[[] for _ in range(n)]
    for u,v in edges: adj[u].append(v);adj[v].append(u)
    col={}
    for s in range(n):
        if not adj[s] or s in col: continue
        col[s]=0; stack=[(s,-1)]
        while stack:
            v,p=stack.pop()
            for w in adj[v]:
                if w==p: continue
                if w in col:
                    if col[w]==col[v]: return False,False
                    # any already visited non-parent edge makes a cycle
                    return False,True
                col[w]=1-col[v];stack.append((w,v))
    return True,True

def main(path):
    S=defaultdict(lambda:dict(n=0,empty=0,nonforest=0,nonbip=0,maxinc=0))
    for B,E,core,sp,ed in read_cases(path):
        deg=[0]*B; adj=[set() for _ in range(B)]
        for u,v in ed: deg[u]+=1;deg[v]+=1;adj[u].add(v);adj[v].add(u)
        t=[4+(1 if E and v==sp else 0)-deg[v] for v in range(B)]
        U=[v for v in range(B) if t[v]>0]
        D=[]
        for ii,p in enumerate(U):
            for q in U[ii+1:]:
                if not path_exact(adj,p,q,2) and not path_exact(adj,p,q,6): D.append((p,q))
        forest,bip=forest_bip(B,D)
        inc=sum(t[v] for v in U if any(v in e for e in D))
        z=S[(B,E)];z['n']+=1;z['empty']+=not D;z['nonforest']+=not forest;z['nonbip']+=not bip;z['maxinc']=max(z['maxinc'],inc)
        if not forest or not bip: raise SystemExit(f'FAIL DQ {B,E,core,sp}')
        if inc>=12: raise SystemExit(f'FAIL quota {B,E,core,sp} inc={inc}')
    for k in sorted(S): print(k,S[k])
    print('PASS D6 CYCLE ELIMINATION DATA')
if __name__=='__main__': main(sys.argv[1] if len(sys.argv)>1 else 'endpoint_cases_d6.txt')
