# CPU Scheduling Research

## Purpose

This directory contains research related to CPU scheduling for ORIGIN OS.

The research focuses on understanding how operating systems select runnable threads for CPU execution and how scheduling behavior can be adapted to different workload conditions.

## Research Areas

Current areas of investigation:

* CPU scheduling fundamentals
* Existing scheduling algorithms
* Linux scheduling
* Windows scheduling
* Adaptive scheduling
* Scheduling on heterogeneous CPUs

## Current Research Direction

ORIGIN is investigating an **Adaptive Scheduling Architecture**.

The current direction is to study whether scheduling behavior can be adjusted using runtime system information such as:

* CPU utilization
* Thread waiting time
* CPU burst behavior
* I/O behavior
* Thread priority
* CPU capacity
* System load

No final scheduling algorithm or architecture has been selected.

## AI / Machine Learning

AI and machine learning are **not currently part of the scheduling architecture**.

They remain a separate research question and will only be considered if research and experimentation demonstrate a measurable benefit that justifies their computational and architectural cost.

## Research Question

> Can a scheduler adapt its behavior to changing workload conditions while maintaining acceptable fairness, responsiveness, and scheduling overhead?
