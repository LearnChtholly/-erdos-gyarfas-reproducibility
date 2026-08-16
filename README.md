# Erdős–Gyárfás Reproducibility

Reproducibility code, certificates, and computer-assisted structural candidate results concerning hypothetical minimal counterexamples to the Erdős–Gyárfás conjecture.

> **Status.** This repository contains **AI-assisted, computer-assisted candidate results**. The proof packages have undergone multiple AI-assisted adversarial checks, reruns, and separately implemented verification routes, but the human maintainer has **not independently reconstructed all mathematical arguments or personally reproduced all computations locally**. Independent human expert verification is being sought.  
> This repository does **not** claim a proof of the Erdős–Gyárfás conjecture, and no priority claim is made here.

The repository is intended primarily as a **code/data/certificate reproducibility companion**. A manuscript may be distributed separately.

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

## 2. Main candidate structural results

### Universal additive \(+8\) bound

The strongest candidate statement represented by the repository is

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

The proof package is split by the value of \(b\):

- \(b \le 3\): uses a known lower bound on the order of a counterexample;
- \(4 \le b \le 7\): exhaustive component-topology enumeration with independent local-cycle implementations;
- \(b=8\): dedicated \(B8+8\) certificates and independent exact-quota / obstruction checks;
- \(b=9,10\): finite \(P8\) endpoint-budget / matching certificates;
- \(b\ge11\): fixed-defect suppression-graph bounds together with parity.

The corresponding code and certificates are under:

```text
universal8/
```

This statement is presented here as an **AI-assisted, computer-assisted theorem candidate pending independent human verification**.

---

### T10

A separately frozen package contains a computer-assisted candidate proof of the stronger restricted-range statement

\[
b\ge14
\quad\Longrightarrow\quad
\boxed{a\ge2b+10}.
\]

The current main package is **T10 v1.1**. Its final path-elimination step uses a small endpoint-budget certificate

\[
\lambda:E(Q)\to\{0,1,2\}.
\]

For each dangerous finite case, every locally admissible path with endpoints \(e,f\) and \(r\) internal vertices is certified to satisfy

\[
r\le\lambda(e)+\lambda(f),
\]

while

\[
\sum_{e\in E(Q)}\lambda(e)<y,
\]

where \(y\) is the total number of internal degree-two vertices in \(G[V_3]\).

After cycle components have been eliminated, the endpoint tokens are used exactly once across the path decomposition, so the candidate deduction is

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

labeled cases. The maximum endpoint-budget sums recorded by the package are

| Sector \((b,E)\) | \(\max \sum_e\lambda(e)\) | \(y\) |
|---|---:|---:|
| \((14,0)\) | 13 | 16 |
| \((14,1)\) | 15 | 17 |
| \((15,0)\) | 4 | 16 |
| \((15,1)\) | 6 | 17 |

The T10 material is under:

```text
t10/
```

---

## 3. Suppression-graph framework

A recurring object in the candidate proofs is a suppression graph \(Q\) on the high-degree vertices \(B\).

Let \(F=G[A]\), and let

- \(x\) be the number of vertices of \(A\) with degree \(1\) in \(F\);
- \(y\) the number with degree \(2\);
- \(z\) the number with degree \(3\).

Define the total high-degree excess

\[
E=\sum_{v\in B}(d_G(v)-4).
\]

The proof packages use the suppression construction to obtain

\[
e(Q)=x
\]

and the identity

\[
a=4b+E+z-e(Q),
\]

together with the parity relation

\[
e(Q)\equiv z\pmod 2.
\]

They also use cycle lifting from \(Q\) to \(G\) and 2-degeneracy consequences of minimality. These mathematical interfaces are part of what independent external reviewers are invited to check.

---

## 4. Repository layout

The repository is organized approximately as follows:

```text
.
├── README.md
├── STATUS.md
├── SHA256SUMS.txt
│
├── t10/
│   ├── README.md
│   ├── THEOREM_STATEMENT_v1.1.md
│   ├── EXPERT_REVIEW_SUMMARY_v1.1.md
│   ├── VALIDATION_REPORT_v1.1.md
│   ├── endpoint_cases.txt
│   ├── endpoint_budget_certificate.csv
│   ├── endpoint_budget_checker.cpp
│   ├── endpoint_budget_verifier_direct_cycles.cpp
│   ├── generate_endpoint_cases.py
│   ├── reproduce_v1_1.sh
│   └── legacy_v1_0/
│
└── universal8/
    ├── p8/
    ├── b8plus8/
    └── small_b/
```

The manuscript is not required to be present in this code repository. See the local README files for the exact contents and reproduction commands in each module.

---

## 5. Independent computational cross-checks

A design goal of the repository is to avoid relying on a single implementation.

### T10 v1.1

The endpoint-budget certificates are checked by two separately written local-cycle verifiers:

1. an exact-length alternate-path implementation;
2. a direct simple-cycle implementation enumerating \(C_4,C_8,C_{16}\).

Both are intended to cover all 1943 dangerous cases.

The older frozen T10 v1.0 package is retained as a substantially different supplementary verification route using:

- 97,345 local options;
- static dead-state DAG certificates;
- an independently generated exact-cover dynamic program.

The v1.0 machinery is not intended to be a logical dependency of the v1.1 endpoint-budget deduction.

### Universal \(+8\)

Different finite ranges use multiple reconstruction and verification methods, including:

- graph-atlas reconstruction for the small-\(b\) cases;
- alternate-path and direct-cycle implementations;
- endpoint-budget / charging certificates;
- exact-quota searches;
- fixed-defect reconstruction and bootstrapping.

These are **computational cross-checks**, not a substitute for independent human mathematical review of the reductions connecting the finite computations to the stated candidate theorems.

---

## 6. Reproducibility

The repository includes frozen input data, certificates, checkers, and reproduction scripts.

To verify the integrity of the repository files against this release:

```bash
sha256sum -c SHA256SUMS.txt
```

`SHA256SUMS.txt` covers every tracked file in this package except the checksum file itself.

Typical requirements are:

- Python 3;
- NetworkX;
- a C++17-compatible compiler (`g++` was used in the recorded runs).

For C++ programs, a typical compilation command is:

```bash
g++ -O2 -std=c++17 verifier.cpp -o verifier
```

Please consult the README or reproduction instructions inside each proof directory for the exact commands.

---

## 7. Verification status

The intended status terminology in this repository is:

- **frozen**: the corresponding package is versioned and should not be silently modified;
- **AI-assisted internally checked**: mathematical reductions, certificate logic, and code have undergone multiple AI-assisted adversarial reviews, reruns, and separately implemented checks;
- **independent human verification pending**: the human maintainer has not independently reconstructed all arguments or personally reproduced all computations locally, and independent expert review has not yet been completed;
- **no priority claim**: failure to locate a predecessor is not treated as proof of novelty.

In particular:

- **T10 v1.1** is the current frozen main T10 candidate-proof package;
- **T10 v1.0** is retained as supplementary computational cross-checking material;
- **Universal \(+8\)** has undergone multiple AI-assisted internal checks but is still presented conservatively pending independent human verification and literature/priority checking.

Some archived audit files use phrases such as **“human proof,” “human proof skeleton,” or “human/computer interface.”** In those files, “human” refers to a **human-readable mathematical proof layer**, not to independent authorship or verification by the human maintainer.

---

## 8. AI assistance and disclosure

AI systems, including large language models used as research and coding assistants, played a central role in:

- mathematical exploration and proof development;
- finite-search and certificate design;
- verification-code generation and refactoring;
- adversarial checking and alternative implementations;
- computational reruns;
- preparation of manuscript and repository documentation.

The human maintainer selected the problem, provided high-level direction, chose which outputs to preserve and release, and is responsible for accurately disclosing the status of the project.

At the time of this release, the human maintainer has **not independently reconstructed all mathematical arguments and has not personally reproduced all computations in a local environment**.

Accordingly, the mathematical statements in this repository should be treated as **AI-assisted, computer-assisted candidate results pending independent human verification**. The code, certificates, frozen inputs, hashes, and reproduction instructions are public specifically so that external reviewers can check the claims without having to trust the AI systems that helped generate them.

---

## 9. What is not claimed

This repository does **not** claim that the Erdős–Gyárfás conjecture has been solved.

All candidate statements concern the structure of a **hypothetical minimal counterexample**.

The repository also does not currently claim priority for the structural inequalities reported here. Literature searches have been conducted, but external subject-matter expertise is requested before making novelty claims.

---

## 10. External review

External reviewers are particularly invited to examine:

1. the suppression identity, simplicity and 2-degeneracy of \(Q\), and the cycle-lifting argument;
2. the fixed-defect bootstrapping arguments;
3. whether each finite local model is genuinely a relaxation or exhaustive representation of the relevant true configurations;
4. completeness of the small-\(b\) topology enumeration;
5. independence and correctness of the finite verification implementations;
6. whether equivalent or stronger structural inequalities already appear in the literature.

Bug reports, counterexamples, reproduction failures, and references to prior work are especially welcome.

---

## 11. Citation and release status

Until a formal archival citation is available, please cite the repository together with the exact Git commit or release tag used.

Useful release tags may include, for example:

```text
t10-v1.1
universal8-review-v0.1
```

No software license is currently asserted by this README. If a `LICENSE` file is added later, that file should be treated as the authoritative license statement.

---

## Acknowledgements

This project builds on previous work on the Erdős–Gyárfás conjecture, including structural results on minimal counterexamples and computational investigations of power-of-two cycles.

Detailed references and provenance information are provided in the accompanying proof notes and subdirectory documentation.
