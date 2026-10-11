# HKA: Heterogeneous Kernel Architecture

**Part of:** ORIGIN OS
**Goal:** Take the best ideas from existing kernel designs (monolithic, microkernel, exokernel, capability-based, language-based) and combine them in one coherent architecture.

---

## 1. Core Principle

> The kernel provides **mechanism and enforcement**. User space provides **policy and most services**. A subsystem's placement (kernel or user space) can change **without changing its interface**.

---

## 2. Design Goals

1. **Small trusted core**: minimize the code that can break the whole system.
2. **Security by default**: all access goes through capabilities/handles.
3. **Performance where it matters**: hot-path drivers can run in kernel space.
4. **Isolation where it matters**: untrusted or complex components run in user space.
5. **One uniform interface**: moving a component across the boundary must not change how it is called.
6. **Debuggable**: serial logging, panic dumps, and tracing from day one.

---

## 3. Layers

### 3.1 Kernel Core (always in kernel space)

| Component | Notes |
|---|---|
| CPU setup | GDT, TSS, IDT, exceptions |
| Interrupts and timers | Local APIC, I/O APIC, APIC timer/HPET |
| Physical memory manager | Frame allocator (bitmap or free-list) |
| Virtual memory mechanism | Page tables, address spaces, higher-half kernel |
| Scheduler mechanism | Context switch, thread states, basic round-robin; policy pluggable |
| IPC | Fast message passing and shared-memory rings |
| Capability/handle table | Central permission check for every resource |
| Syscall entry | `syscall`/`sysret` |
| Early console | Serial (COM1) and framebuffer text |

### 3.2 Movable Layer (kernel OR user space)

These sit behind the uniform ring-buffer interface (see section 4).

| Component | In kernel when... | In user space when... |
|---|---|---|
| Storage drivers (NVMe, AHCI, virtio-blk) | Trusted and performance-critical | Third-party or experimental |
| Network drivers and stack | Hot path, high throughput | Untrusted or needs sandboxing |
| Filesystems | Boot/root filesystem | Additional or untrusted filesystems |
| Input drivers (PS/2, USB HID) | Early boot | Normal operation |
| USB, Bluetooth stacks | Rarely | Usually (complex, bug-prone) |

### 3.3 User Space (always)

- Window system / compositor
- Shell and applications
- Service manager and name service
- Policy services (security policy, quotas)
- Crypto and key storage (isolated domains)
- AI components (see section 6)

---

## 4. Uniform Interface (the key design decision)

All movable components communicate through **shared-memory submission/completion rings** (similar to io_uring).

- A client submits requests to a ring.
- The service (in kernel or user space) consumes requests and posts completions.
- Because the interface is identical, a driver can **move across the boundary** without changing its clients.

**Rule:** no direct function calls between movable components. Everything goes through rings or IPC messages.

---

## 5. Security Model

- **Capabilities/handles** are the only way to reference a resource.
- Every syscall validates the handle and its rights.
- Rights can be **reduced** when passed on, never increased.
- Each process has its own address space.
- User-space drivers receive only the capabilities they need (specific MMIO ranges, IRQ lines, DMA buffers).
- Use the IOMMU when available to contain DMA from user-space drivers.

---

## 6. AI Components

- Never run in kernel space.
- Run in an isolated user domain.
- Can only **suggest** actions through a capability-checked interface.
- A bug or bad output must not be able to corrupt the kernel or bypass permissions.

---

## 7. Influences

| Design | What HKA borrows |
|---|---|
| Microkernel (seL4, L4) | Minimal core, capabilities, fast IPC |
| Monolithic (Linux) | In-kernel fast paths, mature driver model |
| Exokernel | Mechanism without policy; user-level resource management |
| Zircon / Fuchsia | Handles, kernel objects, user-space driver framework |
| Language-based (Singularity, Redox) | Memory-safe components; consider Rust for risky parts |

---

## 8. Risks

- Combining designs can bring the **complexity of all of them**.
- Fast and isolated paths can turn into **two separate codebases**.
- Mitigation: choose **one IPC/ring abstraction early** and force everything through it.

---

## 9. Roadmap

1. Serial logging, `kprintf`, panic handler
2. GDT, IDT, exception handlers
3. Physical memory manager and paging
4. Kernel heap
5. APIC, timer, keyboard
6. Kernel threads and scheduler
7. IPC and capability table
8. User mode (ring 3), syscalls, ELF loader, first shell
9. **First HKA test:** move one driver (keyboard) from kernel to user space through the ring interface
10. Storage, filesystem, networking
11. GUI and AI services

---

## 10. Open Questions

- Exact ring buffer format (entry layout, wakeup/notification mechanism)
- Capability representation (index into per-process table vs. object pointers)
- Scheduling policy hooks: what can user space influence?
- Which language(s) for kernel components: C++ only, or C++ plus Rust?