#!/usr/bin/env python3
import json, sys
from pathlib import Path
base=Path(__file__).resolve().parent
src=Path(sys.argv[1]) if len(sys.argv)>1 else base/'eg_suppression_core_data.json'
out=Path(sys.argv[2]) if len(sys.argv)>2 else base/'endpoint_cases_regenerated.txt'
d=json.loads(src.read_text())
rows=[]
for B,key in [(14,'T14_cores'),(15,'T15_descendants_with_pair_cover_certificate')]:
    for rec in d[key]:
        edges=[tuple(e) for e in rec['edges']]
        deg=[0]*B
        for u,v in edges: deg[u]+=1;deg[v]+=1
        if max(deg)<=4:
            rows.append((B,0,rec['id'],-1,edges))
        for w in range(B):
            if all(deg[v] <= 4+(v==w) for v in range(B)):
                rows.append((B,1,rec['id'],w,edges))
rows.sort(key=lambda r:(r[0],r[1],r[2],r[3]))
counts={}
with out.open('w') as f:
    f.write('ENDPOINT_CASES_V1\n')
    for B,E,cid,sp,edges in rows:
        counts[(B,E)]=counts.get((B,E),0)+1
        f.write(f'CASE {B} {E} {cid} {sp}\n')
        f.write('EDGES '+' '.join(f'{u},{v}' for u,v in edges)+'\nENDCASE\n')
    f.write('END\n')
print('TOTAL',len(rows))
for k in sorted(counts): print('SECTOR',*k,counts[k])
