# Mean Burst Adaptive Quantum

## Status

**Research / Candidate Mechanism**

Mean Burst Adaptive Quantum (MBAQ) is a candidate mechanism for dynamically determining the Round Robin time quantum in OSFDS.

## Candidate 1 — Square-Root Mean Burst Quantum

$$
Q_1 = \sqrt{\frac{\sum B_i}{N}}
$$

Where:

* $B_i$ = estimated CPU burst of each runnable thread
* $N$ = number of runnable threads

### Advantages

* Simple to calculate.
* Reduces the magnitude of the average CPU burst.
* Does not require an additional tuning parameter.

### Limitations

* Depends on accurate CPU burst estimates.
* The square-root transformation has not been experimentally justified.
* May produce a quantum that is too small for some workloads.

---

## Candidate 2 — Scaled Mean Burst Quantum

$$
Q_2 = \alpha \left(\frac{\sum B_i}{N}\right)
$$

Where:

* $B_i$ = estimated CPU burst of each runnable thread
* $N$ = number of runnable threads
* $\alpha$ = scaling parameter

Initial parameter search space:

$$
\alpha \in \{0.1, 0.2, 0.3, \ldots, 0.9\}
$$

### Advantages

* Provides direct control over the resulting time quantum.
* Allows experimental tuning of \(\alpha\).
* Can be extended to runtime adaptation of \(\alpha\).

### Limitations

* Requires selecting an appropriate value of \(\alpha\).
* Performance may vary across workloads.
* Requires experimental evaluation to determine an effective parameter range.
* Depends on accurate CPU burst estimates.

---

## Evaluation

Both candidates require experimental evaluation using:

* Waiting time
* Response time
* Turnaround time
* Context-switch overhead
* Throughput

The same workloads should be evaluated across different parameter values where applicable.

## Current Status

Neither candidate has been selected as the final OSFDS mechanism.

Further experimentation is required to determine whether either approach provides useful scheduling behavior.
