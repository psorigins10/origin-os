# ORIGIN OS — Kernel Architecture Cheat Sheet

> Goal: Compare existing architectures before designing ORIGIN OS.
> Do not treat any architecture as "best" — understand its trade-offs.

---

## 1. Master Comparison

| Architecture | Core Idea | Kernel Size | Where Services Run | Isolation | Performance | Flexibility | Complexity | Main Strength | Main Weakness |
|---|---|---|---|---|---|---|---|---|---|
| **Monolithic** | Most OS services inside one kernel | Large | Kernel space | Low–Medium | High | High | High | Direct communication | Large privileged codebase |
| **Microkernel** | Keep only essential mechanisms in kernel | Small | Mostly user space | High | Medium–High | High | High | Fault isolation | IPC/coordination overhead |
| **Hybrid** | Combine monolithic and microkernel ideas | Medium–Large | Kernel + user space | Medium | High | High | High | Balance of approaches | Trade-offs remain |
| **Exokernel** | Kernel protects/allocates resources; software controls usage | Very small | Application/library OS | High | Potentially High | Very High | High | Hardware/resource control | More responsibility for software |
| **Nanokernel** | Extremely minimal privileged core | Extremely small | Mostly outside kernel | High | Potentially High | High | High | Tiny TCB | System-wide complexity |
| **Unikernel** | Application + required OS components become one image | Tiny/specialized | Single specialized image | High between instances | High | Low | Medium–High | Specialization | Poor general-purpose flexibility |
| **Multikernel** | Multiple cooperating kernel instances manage a many-core machine | Multiple small kernels | Multiple kernel domains | Potentially High | High for local work | High | Very High | Scalability/locality | Communication & coordination |

---

# 2. Architecture-by-Architecture

## Monolithic Kernel

### Core Idea
Most major OS services run inside one privileged kernel address space.

### Typical Components
- Scheduler
- Memory management
- Filesystems
- Networking
- Device drivers
- Process management

### Advantages
- Direct communication between components
- High performance
- Mature hardware support
- Flexible

### Trade-offs
- Large trusted computing base
- Weak fault isolation
- Kernel bugs can affect the entire system
- Large codebase

### Security
More components run with high privileges, increasing the potential impact of a compromised kernel component.

### Performance
Generally strong because kernel components can communicate directly.

### Complexity
The kernel can become extremely large and interconnected.

### Examples
- Linux
- FreeBSD
- OpenBSD
- NetBSD

### ORIGIN Lesson
**Keep the performance benefits of direct communication without unnecessarily increasing the privileged attack surface.**

---

# Microkernel

### Core Idea
Keep only essential mechanisms inside the kernel.

Other services run in user space.

### Kernel Usually Handles
- Scheduling
- Basic memory management
- Address spaces/protection
- IPC
- Interrupt handling

### Services Outside Kernel
- Filesystems
- Device drivers
- Networking
- Other OS services

### Advantages
- Stronger fault isolation
- Smaller trusted computing base
- Modular design
- Failed services can potentially be restarted independently

### Trade-offs
- IPC overhead
- More context switching
- More communication complexity
- Distributed services are harder to coordinate

### Security
Smaller privileged core can reduce the amount of code that must be trusted.

### Performance
Potential overhead from IPC and protection-boundary crossings.

### Complexity
Kernel may be simpler, but the overall system can become more complex.

### Examples
- MINIX 3
- seL4
- QNX

### ORIGIN Lesson
**Isolation is valuable, but communication between isolated components has a cost.**

---

# Hybrid Kernel

### Core Idea
Combine ideas from monolithic and microkernel architectures.

Some functionality remains in kernel space while other components can be modularized or isolated.

### Advantages
- Can retain direct communication for performance
- Can introduce modularity/isolation
- Flexible architectural choices

### Trade-offs
- Large privileged components may remain
- Isolation is not necessarily as strong as a pure microkernel
- Architecture can become complicated
- Trade-offs are moved rather than eliminated

### Security
Depends heavily on which components run with kernel privileges.

### Performance
Can be high because important components can communicate directly.

### Complexity
Potentially high because the system combines multiple architectural approaches.

### Examples
- Windows NT family
- Apple XNU

### ORIGIN Lesson
**Combining architectures can balance trade-offs, but does not automatically eliminate them.**

---

# Exokernel

### Core Idea
The kernel provides protected access to hardware resources while allowing applications or library OSes to decide how those resources are used.

### Kernel Focus
- Resource allocation
- Protection
- Hardware multiplexing

### Advantages
- Very high application control
- Application-specific optimization
- Minimal abstraction imposed by the kernel
- Potentially high performance

### Trade-offs
- More responsibility for applications/library OSes
- More low-level programming
- Resource management becomes harder
- Security boundaries become more complicated

### Security
The kernel still enforces protection, but applications receive much more low-level control.

### Performance
Can be very high because unnecessary abstractions can be avoided.

### Complexity
Kernel may be small, but complexity moves into library OSes and applications.

### Examples
- MIT Exokernel
- Nemesis (related research direction)

### ORIGIN Lesson
**Separate resource protection from resource policy whenever useful.**

---

# Nanokernel

### Core Idea
Make the privileged kernel layer extremely small and keep only minimal mechanisms inside it.

### Advantages
- Very small trusted computing base
- Minimal privileged code
- Potentially easier verification
- Strong separation of responsibilities

### Trade-offs
- More functionality outside the kernel
- Communication complexity
- System-wide design becomes harder
- Performance can depend heavily on implementation

### Security
A smaller privileged core can reduce the amount of code that must be trusted.

### Performance
Potentially good, but moving functionality outside the kernel can introduce communication overhead.

### Complexity
Kernel complexity decreases, but system complexity can increase.

### Examples
Nanokernels are primarily associated with research and specialized systems rather than mainstream desktop OSes.

### ORIGIN Lesson
**Minimize privileged code, but don't simply push complexity somewhere else without measuring the result.**

---

# Unikernel

### Core Idea
Combine one application with only the OS functionality it needs into a specialized image.

### Structure

```text
Application
    +
Required OS components
    +
Required libraries/drivers
    ↓
One specialized image