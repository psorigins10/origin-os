# Adaptive Scheduling

## Status

**Research / Investigation**

ORIGIN OS is investigating adaptive CPU scheduling as a potential scheduling approach.

## Background

CPU scheduling determines which runnable thread is selected for execution on a CPU.

Traditional scheduling algorithms use predefined rules to make this decision. Examples include Round Robin, priority scheduling, and fair scheduling approaches.

Adaptive scheduling investigates whether these decisions can change according to the current state of the system.

## Research Direction

The proposed direction is to allow the scheduler to observe runtime conditions and adjust its scheduling behavior accordingly.

Possible runtime information includes:

* CPU utilization
* Thread waiting time
* CPU burst behavior
* I/O behavior
* Thread priority
* CPU capacity
* System load

Possible scheduling parameters that could be adapted include:

* Time quantum
* Priority
* CPU selection
* Fairness
* Latency behavior

No specific adaptation mechanism has been selected.

## Research Question

> Can scheduling behavior be adapted to changing workload conditions without introducing excessive overhead, unfairness, or instability?

## AI / Machine Learning

AI and machine learning are not part of the current adaptive scheduling design.

The current research focuses on conventional scheduling mechanisms and measurable runtime information.

AI or machine learning may be investigated separately if future research identifies a scheduling problem where prediction or learned behavior provides a measurable benefit.

## Current Position

Adaptive scheduling is a research direction, not a finalized ORIGIN OS architecture.

Further research and experimentation are required before selecting an implementation or scheduling policy.
