# OSFDS (ORIGIN Scheduling Fairness and Dynamic Scheduling)

**ORIGIN Adaptive Scheduling Architecture**

## Status

**Research / Proposed Architecture**

OSFDS is the current proposed scheduling architecture for ORIGIN OS.

It is designed to investigate whether a scheduler can adapt its scheduling behavior according to runtime system conditions rather than relying on a single fixed scheduling policy.

## Objective

OSFDS aims to make scheduling decisions based on the current state and behavior of runnable threads and available CPUs.

The architecture focuses on:

* Fair CPU allocation
* Thread responsiveness
* Efficient CPU utilization
* Runtime workload adaptation
* Support for heterogeneous CPUs

## Basic Model

```text
Runnable Threads
       ↓
Runtime Observation
       ↓
Scheduling Decision
       ↓
Thread Selection
       ↓
CPU Selection
       ↓
Execution
```

The scheduler observes runtime information and uses it to make scheduling decisions.

## Runtime Information

Potential scheduling inputs include:

* CPU utilization
* Thread waiting time
* CPU burst behavior
* I/O behavior
* Thread priority
* CPU capacity
* System load

The exact set of inputs has not yet been finalized.

## Scheduling Behavior

OSFDS may dynamically adjust scheduling behavior based on observed system conditions.

Potentially adaptive parameters include:

* Time quantum
* Priority
* CPU selection
* Fairness behavior
* Latency behavior

No specific adaptation mechanism has been finalized.

## Existing Scheduling Concepts

OSFDS research draws from established scheduling concepts, including:

* Round Robin time sharing
* Fair scheduling
* EEVDF scheduling concepts
* Priority-based scheduling
* CPU capacity awareness

These concepts are being investigated as potential components or references rather than being assumed to form the final design.

## AI / Machine Learning

AI and machine learning are **not part of OSFDS**.

OSFDS currently uses conventional scheduling mechanisms and runtime system information.

AI or machine learning may be investigated separately if future experiments identify a scheduling problem where learned or predictive behavior provides a measurable benefit.

## Research Question

> Can OSFDS adapt scheduling behavior to changing workload conditions while maintaining acceptable fairness, responsiveness, and scheduling overhead?

## Current Constraints

OSFDS must avoid introducing adaptation overhead that outweighs its scheduling benefits.

The architecture must also consider:

* Scheduling overhead
* Fairness
* Responsiveness
* Stability
* Predictability
* CPU utilization

## Current State

OSFDS is a **research architecture**.

The scheduling policy, adaptation mechanism, runtime metrics, and implementation details remain subject to experimentation and evaluation.
