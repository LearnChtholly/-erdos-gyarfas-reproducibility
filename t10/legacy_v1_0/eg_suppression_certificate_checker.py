
#!/usr/bin/env python3
import json, itertools, hashlib
from pathlib import Path

DATA = Path(__file__).resolve().with_name("eg_suppression_core_data.json")

def adj_from_edges(n, edges):
    adj=[set() for _ in range(n)]
    seen=set()
    for e in edges:
        assert len(e)==2
        u,v=e
        assert 0<=u<n and 0<=v<n and u!=v
        a,b=sorted((u,v))
        assert (a,b) not in seen
        seen.add((a,b))
        adj[u].add(v); adj[v].add(u)
    return adj

def is_2_degenerate(adj):
    n=len(adj)
    alive=[True]*n
    deg=[len(adj[v]) for v in range(n)]
    left=n
    while left:
        found=-1
        for v in range(n):
            if alive[v] and deg[v] <= 2:
                found=v; break
        if found<0:
            return False
        alive[found]=False; left-=1
        for w in adj[found]:
            if alive[w]:
                deg[w]-=1
    return True

def path_exists_exact(adj,p,q,L):
    used={p}
    def dfs(v,d):
        if d==L:
            return v==q
        for w in adj[v]:
            if w in used:
                continue
            if w==q and d<L-1:
                continue
            used.add(w)
            if dfs(w,d+1):
                return True
            used.remove(w)
        return False
    return dfs(p,0)

def valid_path(adj,path,L,p,q):
    if len(path)!=L+1 or path[0]!=p or path[-1]!=q:
        return False
    if len(set(path))!=len(path):
        return False
    return all(path[i+1] in adj[path[i]] for i in range(L))

def has_cycle_length(adj,L):
    n=len(adj)
    # Anchor each cycle at its smallest vertex, search both directions;
    # existence only, so duplicates do not matter.
    for s in range(n):
        used={s}
        def dfs(v,d):
            if d==L:
                return v==s
            for w in adj[v]:
                if d==L-1:
                    if w==s:
                        return True
                    continue
                if w==s or w in used or w<s:
                    continue
                used.add(w)
                if dfs(w,d+1):
                    return True
                used.remove(w)
            return False
        for w in adj[s]:
            if w<=s: continue
            used.add(w)
            if dfs(w,1): return True
            used.remove(w)
    return False

data=json.loads(DATA.read_text(encoding="utf-8"))

# T14: validate every core and exact legal-pair list.
assert len(data["T14_cores"]) == 852
sum_legal=0
for rec in data["T14_cores"]:
    adj=adj_from_edges(14,rec["edges"])
    assert sum(len(x) for x in adj)//2 == 20
    assert is_2_degenerate(adj)
    assert not has_cycle_length(adj,4)
    assert not has_cycle_length(adj,8)
    exact=[]
    for p,q in itertools.combinations(range(14),2):
        if not path_exists_exact(adj,p,q,2) and not path_exists_exact(adj,p,q,6):
            exact.append([p,q])
    assert exact == rec["legal_degree2_attachment_pairs_to_15"]
    assert len(exact) == rec["legal_pair_count"]
    sum_legal += len(exact)
assert sum_legal == 708

# T15: validate every descendant and every pair-cover witness.
assert len(data["T15_descendants_with_pair_cover_certificate"]) == 168
total_pairs=0
for rec in data["T15_descendants_with_pair_cover_certificate"]:
    adj=adj_from_edges(15,rec["edges"])
    assert sum(len(x) for x in adj)//2 == 22
    assert is_2_degenerate(adj)
    assert not has_cycle_length(adj,4)
    assert not has_cycle_length(adj,8)
    cert=rec["pair_cover_certificate"]
    assert len(cert)==105
    seen=set()
    for item in cert:
        p,q=item["pair"]
        assert 0<=p<q<15 and (p,q) not in seen
        seen.add((p,q))
        L=item["blocking_path_length"]
        assert L in (2,6)
        assert valid_path(adj,item["blocking_path"],L,p,q)
        total_pairs+=1
    assert len(seen)==105

assert total_pairs == 17640

print("PASS")
print("852 T14 cores validated.")
print("708 exact legal T14->T15 attachment pairs validated.")
print("168 T15 descendants validated.")
print("17640/17640 T15 vertex pairs carry a certified simple path of length 2 or 6.")
print("Therefore no degree-2 extension to (16,24) can remain C4,C8-free.")
print("data_sha256 =", hashlib.sha256(DATA.read_bytes()).hexdigest())
