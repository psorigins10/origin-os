# BTPA

**Burst Time Prediction Algorithm**

## Status

**Research / Candidate Mechanism**

BTPA is a candidate mechanism for estimating the next CPU burst of a runnable thread using its previous CPU burst history.

## Prediction Model

BTPA uses exponential averaging:

$$
\tau_{n+1} = \alpha_{burst} t_n + (1-\alpha_{burst})\tau_n
$$

Where:

- $t_n$ = actual CPU burst time of the current burst
- $\tau_n$ = previous CPU burst prediction
- $\tau_{n+1}$ = updated CPU burst prediction
- $\alpha_{burst}$ = burst prediction weighting parameter

## Behavior

A larger $\alpha_{burst}$ gives greater weight to recent CPU burst behavior.

A smaller $\alpha_{burst}$ gives greater weight to previous predictions.

## Example

Given:

$$
t_n = 4ms
$$

$$
\tau_n = 8ms
$$

$$
\alpha_{burst} = 0.5
$$

Then:

$$
\tau_{n+1} = (0.5)(4) + (1-0.5)(8)
$$

$$
\tau_{n+1} = 6ms
$$

## Advantages

- Simple computational model.
- Low runtime overhead.
- Adapts predictions based on recent execution behavior.
- Suitable for per-thread burst estimation.

## Limitations

- Cannot know future CPU bursts exactly.
- Prediction quality depends on historical behavior.
- Sudden workload changes may reduce prediction accuracy.
- Requires maintaining prediction state for each thread.
- Requires selection and evaluation of $\alpha_{burst}$.

## OSFDS Integration

BTPA is intended to provide estimated CPU burst values to the Mean Burst Adaptive Quantum mechanism.

```text
CPU Burst History
       ↓
      BTPA
       ↓
Estimated CPU Bursts
       ↓
     MBAQ
       ↓
Adaptive Time Quantum
       ↓
   Round Robin
```

## Current Status

BTPA is a candidate mechanism and has not yet been experimentally evaluated.