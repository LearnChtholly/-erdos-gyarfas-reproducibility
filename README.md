[Uploading README(3).md…]()
# Erdős–Gyárfás Reproducibility

Reproducibility code, certificates, and computer-assisted structural results concerning minimal counterexamples to the Erdős–Gyárfás conjecture.

> **Status.** These results have been internally adversarially reviewed and independently reproduced. External human review is ongoing.  
> This repository does **not** claim a proof of the Erdős–Gyárfás conjecture, and no priority claim is made here.

---

## 1. Background

The Erdős–Gyárfás conjecture asks whether every finite simple graph of minimum degree at least three contains a cycle whose length is a power of two.

Throughout this repository, a **minimal counterexample** means a hypothetical counterexample \(G\) chosen first with minimum order and, subject to that, minimum size.

Let

\[
A = V_3(G), \qquad B = V_{\ge 4}(G),
\]

and write

\[
a = |A|, \qquad b = |B|.
\]

The computations and certificates in this repository study structural restrictions that any such hypothetical minimal counterexample would have to satisfy.

---

## 2. Main structural results

### Universal additive \(+8\) bound

The strongest result currently represented by the repository is the computer-assisted structural statement

\[
\boxed{
|V_3(G)| \ge 2|V_{\ge4}(G)| + 8.
}
\]

Equivalently,

\[
\boxed{
a \ge 2b+8.
}
\]

The proof is split by the value of \(b\):

- \(b \le 3\): follows from the known lower bound on the order of a counterexample;
- \(4 \le b \le 7\): exhaustive component-topology enumeration with independent local-cycle implementations;
- \(b=8\): dedicated \(B8+8\) certificates and independent exact-quota / obstruction checks;
- \(b=9,10\): finite \(P8\) endpoint-budget / matching certificates;
- \(b\ge11\): follows from a fixed-defect extremal bound for the suppression graph together with parity.

The corresponding code and certificates are under:

```text
universal8/
```

This result should presently be regarded as a **computer-assisted theorem candidate undergoing external review**.

---

### T10

A separately frozen result proves that if

\[
b\ge14,
\]

then

\[
\boxed{
a\ge2b+10.
}
\]

The current main proof is **T10 v1.1**, whose final path-elimination step uses a small endpoint-budget certificate

\[
\lambda:E(Q)\to\{0,1,2\}.
\]

For each dangerous finite case, every locally admissible path with endpoints \(e,f\) and \(r\) internal vertices satisfies

\[
r\le\lambda(e)+\lambda(f),
\]

while

\[
\sum_{e\in E(Q)}\lambda(e)<y,
\]

where \(y\) is the total number of internal degree-two vertices in \(G[V_3]\).

Because every endpoint is used exactly once after cycle components have been eliminated,

\[
y
=
\sum_P r(P)
\le
\sum_{e\in E(Q)}\lambda(e)
<y,
\]

a contradiction.

The four dangerous sectors contain

\[
94+1581+14+254=1943
\]

labeled cases. The maximum endpoint-budget sums are

| Sector \((b,E)\) | \(\max \sum_e\lambda(e)\) | \(y\) |
|---|---:|---:|
| \((14,0)\) | 13 | 16 |
| \((14,1)\) | 15 | 17 |
| \((15,0)\) | 4 | 16 |
| \((15,1)\) | 6 | 17 |

Every case therefore satisfies

\[
\sum_e\lambda(e)<y.
\]

The T10 material is under:

```text
t10/
```

---

## 3. Suppression graph framework

A recurring object in the proofs is a suppression graph \(Q\) on the high-degree vertices \(B\).

Let \(F=G[A]\), and let

- \(x\) be the number of vertices of \(A\) with degree \(1\) in \(F\);
- \(y\) the number with degree \(2\);
- \(z\) the number with degree \(3\).

Define the total high-degree excess

\[
E=\sum_{v\in B}(d_G(v)-4).
\]

Suppressing every degree-one vertex of \(F\) produces a simple graph \(Q\) on \(B\) with

\[
e(Q)=x
\]

and the exact identity

\[
\boxed{
a=4b+E+z-e(Q).
}
\]

The handshake lemma in \(F\) also gives

\[
\boxed{
e(Q)\equiv z\pmod 2.
}
\]

Simple cycles in \(Q\) lift to cycles of twice the length in \(G\), and minimality implies that \(Q\) is 2-degenerate.

These facts reduce structural questions about \(G\) to finite extremal and lifting problems for \(Q\).

---

## 4. Repository layout

A typical layout is:

```text
.
├── README.md
├── STATUS.md
├── SHA256SUMS.txt
│
├── t10/
│   ├── README.md
│   ├── EG_T10_note_v1.1.pdf
│   ├── THEOREM_STATEMENT_v1.1.md
│   ├── EXPERT_REVIEW_SUMMARY_v1.1.md
│   ├── VALIDATION_REPORT_v1.1.md
│   ├── endpoint_cases.txt
│   ├── endpoint_budget_certificate.csv
│   ├── endpoint_budget_checker.cpp
│   ├── endpoint_budget_verifier_direct_cycles.cpp
│   ├── generate_endpoint_cases.py
│   ├── ...
│   └── legacy_v1_0/
│
└── universal8/
    ├── p8/
    ├── b8plus8/
    └── small_b/
```

The exact contents may be described in the local README files in each subdirectory.

---

## 5. Independent verification

A deliberate design goal of this repository is to avoid relying on a single search implementation.

Examples include:

### T10 v1.1

The endpoint-budget certificates are checked by two separately written local-cycle verifiers:

1. an exact-length alternate-path implementation;
2. a direct simple-cycle implementation enumerating \(C_4,C_8,C_{16}\).

Both cover all 1943 dangerous cases.

The older frozen T10 v1.0 proof is retained as a substantially different supplementary verification route using:

- 97,345 local options;
- static dead-state DAG certificates;
- an independently generated exact-cover dynamic program.

The v1.0 machinery is **not** logically required by the v1.1 endpoint-budget proof.

### Universal \(+8\)

Different finite ranges use independent reconstruction and verification methods, including:

- graph-atlas reconstruction for the small-\(b\) cases;
- alternate-path and direct-cycle implementations;
- independent endpoint-budget / charging certificates;
- independent exact-quota searches;
- fixed-defect reconstruction and bootstrapping.

See the subdirectory documentation for the exact dependency chain.

---

## 6. Reproducibility

The repository includes frozen input data, certificates, checkers, and reproduction scripts.

Before reproducing results, verify file integrity:

```bash
sha256sum -c SHA256SUMS.txt
```

Individual subdirectories may contain their own manifests and reproduction scripts.

Typical requirements are:

- Python 3;
- a C++17-compatible compiler;
- NetworkX for some independent graph-enumeration checks.

For C++ programs, a typical compilation command is:

```bash
g++ -O2 -std=c++17 verifier.cpp -o verifier
```

Please consult the README or reproduction instructions inside each proof directory for the exact commands.

---

## 7. Proof status

The intended status terminology in this repository is:

- **frozen**: the corresponding proof package is versioned and should not be silently modified;
- **internally validated**: the mathematical reduction, certificate logic, and reproduction code have undergone adversarial internal review and independent reruns;
- **external review ongoing**: independent human expert review has not yet been completed;
- **no priority claim**: absence of a located predecessor is not treated as proof of novelty.

In particular:

- **T10 v1.1** is the current frozen main T10 proof package;
- **T10 v1.0** is retained unchanged as supplementary independent verification;
- **Universal \(+8\)** has passed internal adversarial review but is still being presented conservatively pending external human review and literature/priority checking.

---

## 8. What is not claimed

This repository does **not** claim that the Erdős–Gyárfás conjecture has been solved.

All statements concern the structure of a **hypothetical minimal counterexample**.

The repository also does not currently claim priority for the structural inequalities reported here. Literature searches have been conducted, but external subject-matter expertise is requested before making novelty claims.

---

## 9. External review

External reviewers are particularly invited to examine:

1. the suppression identity, simplicity and 2-degeneracy of \(Q\), and the cycle-lifting argument;
2. the fixed-defect bootstrapping arguments;
3. whether each finite local model is genuinely a relaxation or exhaustive representation of the relevant true configurations;
4. completeness of the small-\(b\) topology enumeration;
5. independence and correctness of the finite verification implementations;
6. whether equivalent or stronger structural inequalities already appear in the literature.

Bug reports, counterexamples, reproduction failures, and references to prior work are especially welcome.

---

## 10. Citation and archival status

A formal citation entry will be added once the external-review and archival status is settled.

If you use the code or certificates before then, please cite this repository together with the exact Git commit or release tag used.

Recommended release tags include, for example:

```text
t10-v1.1
universal8-review-v0.1
```

---

## 11. License

See `LICENSE`.

The license applies to the repository code and associated machine-readable data as specified there. Mathematical statements and attribution should follow normal scholarly citation practice.

---

## Acknowledgements

This project builds on previous work on the Erdős–Gyárfás conjecture, including structural results on minimal counterexamples and computational investigations of power-of-two cycles.

Detailed references and provenance information are provided in the accompanying proof notes and subdirectory documentation.
