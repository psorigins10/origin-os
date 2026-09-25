# ORIGIN OS — Kernel Architecture Cheat Sheet

> Master comparison of existing kernel architecture families.
> Purpose: understand trade-offs before designing ORIGIN OS.

---

## Master Comparison

| Architecture | Core Idea | Kernel Size | Services | Isolation | Performance | Flexibility | Complexity | Main Strength | Main Weakness |
|---|---|---|---|---|---|---|---|---|---|
| **Monolithic** | Most OS services inside one kernel | Large | Mostly kernel space | Low–Medium | High | High | High | Direct communication | Large privileged codebase |
| **Microkernel** | Keep only essential mechanisms in kernel | Small | Mostly user space | High | Medium–High | High | High | Fault isolation | IPC/coordination overhead |
| **Hybrid** | Combine monolithic and microkernel ideas | Medium–Large | Kernel + user space | Medium | High | High | High | Balance of approaches | Trade-offs remain |
| **Exokernel** | Kernel protects/allocates resources; software controls usage | Very small | Application/library OS | High | Potentially High | Very High | High | Resource control | More responsibility |
| **Nanokernel** | Extremely small privileged core | Extremely small | Mostly outside kernel | High | Potentially High | High | High | Small TCB | System complexity |
| **Unikernel** | Application + required OS components form one image | Tiny/specialized | Single specialized image | High between instances | High | Low | Medium–High | Specialization | Low flexibility |
| **Multikernel** | Multiple cooperating kernel instances | Multiple small kernels | Multiple kernel domains | Potentially High | High for local work | High | Very High | Scalability/locality | Coordination complexity |

---

# Monolithic Kernel

**Core idea:** Most major OS services run inside one privileged kernel.

**Strengths**
- Direct communication
- High performance
- Flexible
- Mature hardware support

**Trade-offs**
- Large trusted computing base
- Weak fault isolation
- Kernel bugs can affect the whole system
- Large interconnected codebase

**Examples:** Linux, FreeBSD, OpenBSD, NetBSD

**ORIGIN lesson:** Can we keep direct communication without unnecessarily increasing the privileged attack surface?

---

# Microkernel

**Core idea:** Keep only essential mechanisms in the kernel; move services such as drivers and filesystems into user space.

**Strengths**
- Strong isolation
- Smaller TCB
- Modular services
- Failed services can potentially be restarted

**Trade-offs**
- IPC overhead
- Context switching
- Coordination complexity
- More complicated service communication

**Examples:** MINIX 3, seL4, QNX

**ORIGIN lesson:** Isolation is valuable, but communication between isolated components has a cost.

---

# Hybrid Kernel

**Core idea:** Combine ideas from monolithic and microkernel architectures.

**Strengths**
- Can retain direct communication
- Can introduce modularity/isolation
- Flexible design

**Trade-offs**
- Some components still run with high privileges
- Can remain complex
- Isolation depends on implementation
- Trade-offs are moved rather than eliminated

**Examples:** Windows NT family, Apple XNU

**ORIGIN lesson:** Combining architectures can balance trade-offs but does not automatically eliminate them.

---

# Exokernel

**Core idea:** The kernel mainly provides protection and resource allocation while applications or library OSes decide how resources are used.

**Strengths**
- High hardware/resource control
- Application-specific optimization
- Minimal imposed abstractions
- Potentially high performance

**Trade-offs**
- More responsibility for applications
- More low-level programming
- Resource management becomes harder
- Security boundaries become more complicated

**Examples:** MIT Exokernel research

**ORIGIN lesson:** Separate resource protection from resource policy when useful.

---

# Nanokernel

**Core idea:** Keep the privileged kernel layer extremely small and move more functionality outside it.

**Strengths**
- Very small TCB
- Minimal privileged code
- Potentially easier verification
- Strong separation of responsibilities

**Trade-offs**
- More functionality outside the kernel
- Communication complexity
- System-wide complexity can increase
- Performance depends heavily on implementation

**ORIGIN lesson:** Minimize privileged code without simply moving the complexity somewhere else.

---

# Unikernel

**Core idea:** Combine one application with only the OS components and libraries it needs into a specialized image.

```text
Application
     +
Required OS components
     +
Required libraries/drivers
     ↓
One specialized image
```

**Strengths**
- Small footprint
- Fast startup potential
- Specialized environment
- Reduced unnecessary software
- Potentially smaller attack surface
- Strong isolation between separate instances

**Trade-offs**
- Poor general-purpose flexibility
- Application and OS become tightly coupled
- Updates may require rebuilding the image
- Many images can increase maintenance complexity

**Examples:** MirageOS, IncludeOS, OSv

**ORIGIN lesson:** Specialization can improve efficiency, but static specialization creates maintenance problems.

---

# Multikernel

**Core idea:** Treat a many-core machine as multiple cooperating kernel instances instead of one centralized kernel.

```text
Many-Core Machine

Kernel A → Cores 0–3
Kernel B → Cores 4–7
Kernel C → Cores 8–11
Kernel D → Cores 12–15

        ↕
   Communication
```

**Strengths**
- Scalability on many-core systems
- Local decision making
- Better locality
- Reduced dependence on one centralized kernel
- Potential fault isolation

**Trade-offs**
- Communication overhead
- Coordination complexity
- Global resource management is harder
- Failure recovery is harder
- Maintaining consistency can be difficult

**Example:** Barrelfish

**ORIGIN lesson:** Distributed control can improve scalability, but coordination becomes a fundamental cost.

---

# Recurring Trade-offs

## 1. Performance vs Isolation

```text
More isolation
     ↓
More communication
     ↓
Potential overhead

Less isolation
     ↓
Direct communication
     ↓
Potentially better performance
```

## 2. Centralization vs Distribution

```text
Centralized control
     ↓
Simpler coordination
     ↓
Potential bottlenecks

Distributed control
     ↓
Better scalability/locality
     ↓
Harder coordination
```

## 3. Generalization vs Specialization

```text
General-purpose
     ↓
Flexible
     ↓
More functionality

Specialized
     ↓
Efficient
     ↓
Harder to maintain/adapt
```

## 4. Kernel Size vs System Complexity

```text
Smaller kernel
     ≠
Simpler entire system

Complexity can move outside the kernel.
```

---

# Questions ORIGIN Should Investigate

1. Can strong isolation be achieved without excessive IPC overhead?
2. Can high performance be maintained without putting everything in kernel space?
3. Can resource management become adaptive instead of relying only on static policies?
4. Can an OS automatically specialize itself for different workloads?
5. Can distributed control scale without excessive coordination costs?
6. Can failures be isolated and recovered automatically?
7. Can the trusted computing base remain small?
8. Can the system adapt to changing workloads?
9. Can AI safely assist system-level decisions?
10. Can functionality dynamically move between privilege domains?
11. Can system complexity actually be reduced rather than merely moved?

---

# ORIGIN Research Principle

Do **not** ask:

> "Which existing architecture should ORIGIN copy?"

Ask:

> "What fundamental problems remain after studying existing architectures?"

Research → Identify limitations → Find recurring trade-offs → Form hypotheses → Design experiments → Measure → Analyze → Build → Revise

---

# Current ORIGIN Hypothesis

**Not a final architecture.**

> Can an operating system dynamically adapt its resource management, isolation, and system policies according to workload and system state while maintaining strong security and predictable performance?

This hypothesis must be tested through research and experiments.

---

# Golden Rule

> **Research first. Architecture second. Implementation third.**