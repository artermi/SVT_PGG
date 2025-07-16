# Spatial versus Temporal Randomness in Public Goods Cooperation

**Date:** June 8, 2025

## Core Question

If two public-goods games have the same mean and variance of the synergy factor $r$, does it matter whether the randomness is frozen in space (different sites, fixed in time) or shuffled in time (same for everyone, changes every $\tau$ steps)?

---

## 0. Symbol Key (Quick Reference)

| Symbol           | Description                                                  |
|------------------|--------------------------------------------------------------|
| $r$              | synergy factor                                                |
| $L$              | lattice side length (sites per row)                           |
| $N = L^2$        | total number of agents                                        |
| $G$              | group size (= 5)                                              |
| $r_g(t)$         | synergy factor used by group $g$ at time $t$                  |
| $f(r)$           | probability law from which all $r$-values are drawn           |
| $r_0$            | mean of $f(r)$ (baseline synergy)                             |
| $\sigma^2$      | variance of $f(r)$                                            |
| $s_i(t)$         | strategy of agent $i$ at time $t$ (1=C, 0=D)                  |
| $K$              | selection “temperature” in Fermi rule                         |
| $\tau$          | refresh period for Model B (temporal noise)                  |

---

## 1. Lattice and Group Geometry

### 1.1 Sites and Boundaries

- Use a square lattice with periodic boundaries. Coordinates run $x, y \in \{0, \dots, L - 1\}$.
- Index arithmetic is always modulo $L$.
- Standard system size: $L = 100$ (i.e. $N = 10{,}000$ agents). Also test $L = 150$ and $L = 200$.

### 1.2 Groups

- Each lattice site is the center of one public-goods group. Total groups = total sites = $N$.
- Group: center site plus its four von Neumann neighbors.
- So, group size $G = 5$.
- Each agent belongs to exactly 5 groups.
- Future work: extend to other group sizes.

---

## 2. Strategies and Payoffs

### 2.1 Strategy Variable

$$
s_i(t) = 
\begin{cases}
1 & \text{if agent } i \text{ cooperates at time } t \\
0 & \text{if agent } i \text{ defects at time } t
\end{cases}
$$

### 2.2 Payoff from a Single Group

For agent $i$ in group $g$ at time $t$:

$$
\pi^g_i(t) = r_g(t) \frac{n_C^g(t)}{G} - s_i(t)
$$

where $n_C^g(t) = \sum_{j \in g} s_j(t)$ is the number of cooperators in group $g$.

### 2.3 Total Payoff

Each agent is in 5 groups:

$$
\Pi_i(t) = \sum_{i \in g} \pi^g_i(t)
$$

---

## 3. Strategy Update (Fermi Imitation)

### 3.1 One Monte-Carlo Step

1. Randomly shuffle all $N$ agents.
2. For each agent $i$:
   - Compute $\Pi_i(t)$
   - Choose random neighbor $j$
   - Compute $\Pi_j(t)$
   - Update strategy with probability:

$$
W_{i \leftarrow j}(t) = \left[ 1 + \exp((\Pi_i - \Pi_j)/K) \right]^{-1}
$$

### 3.2 Interpreting $K$

- Small $K$ → almost deterministic copying of better strategy.
- Large $K$ → nearly random imitation.
- $K \to \infty$ → imitation probability approaches 1/2.

---

## 4. Randomness Protocols

All $r$ values drawn from same distribution $f(r)$ with:

- $\mathbb{E}[r] = r_0 > 1$
- $\text{Var}(r) = \sigma^2$

### 4.1 Model A: Spatial (“quenched”) randomness

- At $t = 0$, sample $R_g \sim f(r)$ for each group $g$.
- Freeze in time: $r_g(t) = R_g$ for all $t > 0$.
- Independent across space, constant over time.

### 4.2 Model B: Temporal (“annealed”) randomness

- Choose refresh period $\tau \in \{1, 10, 100, 1000, \infty\}$
- For each block $k = \lfloor t/\tau \rfloor$, draw one $S_k \sim f(r)$
- Set $r_g(t) = S_k$ for all $g$ in that block.

### 4.3 Fairness of Comparison

- Same means and variances ($r_0$, $\sigma^2$)
- Same lattice, update rules, and $K$
- Shared random seed (only r-stream differs)
- Minimized Monte Carlo noise in $\Delta C = C_A^\infty - C_B^\infty$

---

## 5. Distribution Families

All matched to $r_0$, $\sigma^2$:

- **D1 Uniform:** $r \in [r_0 - \sqrt{3\sigma}, r_0 + \sqrt{3\sigma}]$
- **D2 Binary:** $r = r_0 \pm \sigma\sqrt{2}$ (equal probability)
- **D3 Truncated Gaussian:** $r \sim N(r_0, \sigma^2)$, truncated to $r > 1$
- **D4 Shifted Exponential:** $r = 1 + X$, $X \sim \text{Exp}(\lambda)$ with $\lambda = (r_0 - 1)^2 / \sigma^2$

---

## 6. Simulation Schedule

- Initial state: 50% cooperators (random)
- Ensure final state is stationary before evaluation

---

## 7. Stored Data per Sample

- Global cooperation fraction $C(t)$
- Mean and variance of payoffs
- Histogram of cooperator cluster sizes
- Exact $r$ value(s) at each time step
- After run: compute spatial correlation length $\xi$, check for all-C or all-D fixation

---

## 8. First Parameter Grid (Pilot Set)

- $r_0$: 2.0, 2.5, 3.0, 3.5  
- $\sigma$: $\{0.1, \dots, 0.5\} \times r_0$  
- $f(r)$: 4 families listed above  
- $\tau$: 1, 10, 100, 1000, $\infty$  
- $K$: 0.01, 0.05, 0.1, 0.2, 0.5, 1.0, 2.0  
- $L$: 100 (later 150, 200)

