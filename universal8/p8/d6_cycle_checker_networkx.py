#!/usr/bin/env python3
import sys
from collections import defaultdict
import networkx as nx

def read_cases(path):
    L=open(path,encoding='utf-8').read().splitlines(); assert L[0]=='ENDPOINT_CASES_V1'
    out=[]; i=1
    while L[i] != 'END':
        if not L[i]: i+=1; continue
        h=L[i].split(); assert h[0]=='CASE'
        B,E,core,sp=int(h[1]),int(h[2]),h[3],int(h[4])
        eparts=L[i+1].split(); assert eparts[0]=='EDGES'
        ed=[tuple(map(int,x.split(','))) for x in eparts[1:]]
        assert L[i+2]=='ENDCASE'
        out.append((B,E,core,sp,ed)); i+=3
    return out

def has_exact_simple_path(G,s,t,L):
    # independent implementation through NetworkX all_simple_paths cutoff L,
    # filtered to exactly L edges.
    for p in nx.all_simple_paths(G,s,t,cutoff=L):
        if len(p)-1 == L: return True
    return False

def main(path):
    stats=defaultdict(lambda:dict(n=0,empty=0,maxinc=0))
    for idx,(B,E,core,sp,ed) in enumerate(read_cases(path)):
        G=nx.Graph(); G.add_nodes_from(range(B)); G.add_edges_from(ed)
        if G.number_of_edges()!=len(ed): raise SystemExit(f'parallel/loop at {idx}')
        deg=dict(G.degree())
        t=[4+(1 if E and v==sp else 0)-deg[v] for v in range(B)]
        if min(t)<0: raise SystemExit(f'negative t at {idx}')
        if sum(t)!=(12+E): raise SystemExit(f'quota sum at {idx}: {sum(t)}')
        U=[v for v in range(B) if t[v]>0]
        D=nx.Graph(); D.add_nodes_from(U)
        for ii,p in enumerate(U):
            for q in U[ii+1:]:
                if not has_exact_simple_path(G,p,q,2) and not has_exact_simple_path(G,p,q,6):
                    D.add_edge(p,q)
        if not nx.is_forest(D): raise SystemExit(f'DQ not forest at {idx} {core} sp={sp}')
        if not nx.is_bipartite(D): raise SystemExit(f'DQ not bipartite at {idx}')
        inc=sum(t[v] for v in D.nodes if D.degree(v)>0)
        if inc>=12: raise SystemExit(f'incident quota {inc} >=12 at {idx}')
        S=stats[(B,E)]; S['n']+=1; S['empty'] += (D.number_of_edges()==0); S['maxinc']=max(S['maxinc'],inc)
    for k in sorted(stats): print(k,stats[k])
    print('PASS D6 NETWORKX CYCLE CHECK')
if __name__=='__main__': main(sys.argv[1] if len(sys.argv)>1 else 'endpoint_cases_d6.txt')
