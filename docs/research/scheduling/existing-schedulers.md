# Existing Schedulers

## Purpose

This document records existing CPU scheduling approaches relevant to ORIGIN OS research.

## Round Robin

Round Robin is a time-sharing scheduling algorithm.

Runnable threads are placed in a queue and receive CPU time for a defined time slice. When a thread's time slice expires, another runnable thread can be selected.

```text
T1 → T2 → T3 → T4 → T1 → ...
```

Round Robin provides simple time sharing but does not account deeply for differences between workloads.

## Shortest Job First

Shortest Job First (SJF) selects the runnable thread with the shortest expected CPU burst.

For example:

```text
T1: 8 ms
T2: 3 ms
T3: 6 ms
T4: 2 ms
```

The selection order would be:

```text
T4 → T2 → T3 → T1
```

SJF can reduce average waiting time under its classical assumptions, but it depends on estimating future CPU burst length and can cause long-running workloads to wait when shorter workloads continually arrive.

## Linux

Linux uses multiple scheduling classes rather than a single scheduling algorithm.

For normal fair scheduling, modern Linux uses **EEVDF (Earliest Eligible Virtual Deadline First)**.

EEVDF uses scheduling information such as task lag and virtual deadlines when selecting runnable tasks.

Linux also provides other scheduling classes and policies for workloads with different requirements, including real-time and deadline scheduling.

Linux scheduling also considers CPU capacity and task utilization when making CPU-placement decisions, which is particularly relevant on heterogeneous processors.

## Windows

Windows uses a priority-based scheduler and dispatcher.

Scheduling decisions can consider factors including:

* Thread priority
* Time quanta
* Dynamic priority adjustments
* Processor selection
* Processor affinity
* Workload characteristics

Windows can temporarily adjust thread priorities in response to certain events, such as I/O completion or user interaction.

Windows also includes scheduling behavior for heterogeneous processors.

## Relevance to ORIGIN

Existing schedulers demonstrate that scheduling decisions can use more information than simply selecting the next thread from a fixed queue.

Relevant areas for ORIGIN research include:

* Fairness
* Time sharing
* Latency
* Priority
* CPU utilization
* CPU capacity
* Runtime workload behavior

These mechanisms provide reference points for investigating adaptive scheduling in ORIGIN OS.

## References

* Linux kernel scheduler documentation
* Microsoft Windows scheduling documentation
