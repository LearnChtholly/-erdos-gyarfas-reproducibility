# Novelty enemy-check report

**Search date:** 16 August 2026.  
**Purpose:** public-source prior-art search only; this is separate from proof correctness.

## Result

No public source was found in the targeted search that states the conditional additive bound

\[
|V_3(G)|\ge2|V_{\ge4}(G)|+10\qquad(|V_{\ge4}(G)|\ge14),
\]

or an obviously equivalent `+6`, `+8`, or `+10` degree-excess/suppression statement. This is **not a priority claim**. The correct wording is:

> As of the targeted public-source search on 16 August 2026, no prior public source containing the T10 bound or an obvious equivalent was found.

## Important predecessors that must be acknowledged

1. **Klas Markström (2004).** Structural observations on minimal counterexamples, including the independence of vertices of degree at least four; Carr attributes this observation to Markström.
2. **Avery Carr (2026), arXiv:2605.22844.** For a lexicographically minimal counterexample: every proper subgraph has minimum degree at most two (Lemma 0.1); every vertex has a cubic neighbor and the vertices of degree at least four are independent (Corollary 0.1); at least `4/7` of the vertices are cubic (Theorem 0.1).
3. **Public Erdős #64 campaign (30 July 2026).** William Blair's public campaign frontier records Carr's lemmas and a forum `2/3`-density lemma as verified. Strict cubic density greater than `2/3` is equivalent to the additive integer inequality `|V_3| >= 2|V_{>=4}|+1`.
4. **Ulam AI Autoresearch Archive (verified 30 July 2026).** Search indexing for *Cubic Density in Erdős-Gyárfás Counterexamples* reports a bound of at least `ceil((2n+3)/3)` cubic vertices, equivalent to `|V_3| >= 2|V_{>=4}|+3`. The full page/PDF was not retrievable from the present browsing environment, so this is recorded as relevant public prior art rather than used as a proof input.
5. **Julius Tranquilli (2026), arXiv:2608.02675.** Certified computation raises the lower bound for a cubic bipartite counterexample to 60 vertices. This is a different, finite-order/cubic-bipartite direction.

## Sources searched / query families
- Carr paper and citation trail; Markström/Royle/Exoo terminology.
- Erdős Problems / UCSD problem page.
- William Blair `lean-proofs` campaign material and searches for `2/3-density`, `jul059`, suppression and degree-3 density.
- Ulam AI autoresearch archive.
- Formal Conjectures / Lean-related repositories.
- arXiv and general web searches for 2025-2026 results.
- Exact/equivalent forms such as `|V_3|-2|V_{>=4}|`, `2b+3`, `2b+6`, `2b+8`, `2b+10`, `suppression`, `smoothing`, `predominantly cubic`.

## URLs for external reviewer
- Carr: https://arxiv.org/abs/2605.22844
- Erdős problem page: https://mathweb.ucsd.edu/~erdosproblems/erdos/newproblems/PowerOfTwoCycles.html
- Blair campaign frontier: https://github.com/williamjblair/lean-proofs/blob/main/FRONTIER.md
- Ulam AI archive: https://www.ulam.ai/archive.html
- Tranquilli: https://arxiv.org/abs/2608.02675

## Remaining novelty risk
The largest remaining risk is **unindexed or hard-to-search material**: private correspondence, forum comments not indexed by search engines, theses, unpublished notes, conference handouts, or a differently parameterized equivalent inequality. An external graph theorist familiar with Erdős-Gyárfás work should be asked specifically whether they know an equivalent additive degree-excess statement.
