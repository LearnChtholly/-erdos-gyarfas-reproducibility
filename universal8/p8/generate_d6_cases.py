#!/usr/bin/env python3
import json, sys
src=sys.argv[1] if len(sys.argv)>1 else 'eg_fixed_defect_reps.json'
outp=sys.argv[2] if len(sys.argv)>2 else 'endpoint_cases_d6.txt'
R=json.load(open(src))['states']
sets=[(9,'F9_12'),(10,'F10_14')]
counts={}
with open(outp,'w') as out:
    out.write('ENDPOINT_CASES_V1\n')
    for B,key in sets:
        for ci,ed in enumerate(R[key]):
            deg=[0]*B
            for u,v in ed: deg[u]+=1; deg[v]+=1
            core=f'D6_B{B}_{ci:04d}'
            if max(deg)<=4:
                counts[(B,0)]=counts.get((B,0),0)+1
                out.write(f'CASE {B} 0 {core} -1\nEDGES '+' '.join(f'{u},{v}' for u,v in ed)+'\nENDCASE\n')
            for sp in range(B):
                if deg[sp]<=5 and all(deg[v]<=4 for v in range(B) if v!=sp):
                    counts[(B,1)]=counts.get((B,1),0)+1
                    out.write(f'CASE {B} 1 {core} {sp}\nEDGES '+' '.join(f'{u},{v}' for u,v in ed)+'\nENDCASE\n')
    out.write('END\n')
print(counts, 'total', sum(counts.values()))
