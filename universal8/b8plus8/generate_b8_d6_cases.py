#!/usr/bin/env python3
import json,sys
src=sys.argv[1] if len(sys.argv)>1 else 'eg_fixed_defect_reps.json'
outp=sys.argv[2] if len(sys.argv)>2 else 'b8_d6_cases.txt'
R=json.load(open(src))['states'];B=8;counts={}
with open(outp,'w') as f:
    f.write('ENDPOINT_CASES_V1\n')
    for ci,ed in enumerate(R['F8_10']):
        deg=[0]*B
        for u,v in ed:deg[u]+=1;deg[v]+=1
        core=f'D6_B8_{ci:04d}'
        if max(deg)<=4:
            counts[(8,0)]=counts.get((8,0),0)+1
            f.write(f'CASE 8 0 {core} -1\nEDGES '+' '.join(f'{u},{v}' for u,v in ed)+'\nENDCASE\n')
        for sp in range(B):
            if deg[sp]<=5 and all(deg[v]<=4 for v in range(B) if v!=sp):
                counts[(8,1)]=counts.get((8,1),0)+1
                f.write(f'CASE 8 1 {core} {sp}\nEDGES '+' '.join(f'{u},{v}' for u,v in ed)+'\nENDCASE\n')
    f.write('END\n')
print(counts,'total',sum(counts.values()))
