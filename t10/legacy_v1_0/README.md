# EG-T10-proof-v1.0

Frozen reproducibility package for the structural bound

\[
|V_{\ge4}(G)|\ge14\quad\Longrightarrow\quad |V_3(G)|\ge2|V_{\ge4}(G)|+10
\]

for a counterexample to the Erdős-Gyárfás conjecture chosen first with minimum order and then minimum size.

**This package does not claim to solve the Erdős-Gyárfás conjecture. Priority/novelty is not claimed.**

Start with:
- `THEOREM_STATEMENT.md` - frozen statement and scope.
- `EG_T10_note.pdf` / `EG_T10_note.tex` - external-review mathematical note.
- `PROOF_DEPENDENCY_GRAPH.md` - proof architecture.
- `REPRODUCIBILITY_CHECKLIST.md` - verification instructions.
- `NOVELTY_SEARCH_REPORT.md` - public prior-art search and caveats.
- `EXPERT_REVIEW_SUMMARY.md` - one-page expert brief.
- `RISKS.md` - remaining risks.

## Reproduce

```bash
python3 -m pip install -r requirements.txt
./reproduce_all.sh quick
./reproduce_all.sh full
```

`quick` verifies the fixed-defect reconstruction, case partition, cycle-elimination statistics and every static DAG certificate. `full` additionally runs the independent local-option generator and independent exact-cover DP.

See `EXPECTED_OUTPUTS.txt` for required totals and `SHA256SUMS.txt` for file integrity.
