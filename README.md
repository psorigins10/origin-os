# ORIGIN OS

> An experimental x86-64 operating system exploring computer architecture,
> kernel design, security, and adaptive systems.

ORIGIN OS is a long-term systems research and learning project focused on
understanding how computers work from the lowest practical level upward.

The project begins with firmware, bootloaders, CPU architecture, memory,
and kernel development, then expands toward filesystems, drivers,
networking, security, distributed systems, and eventually AI/ML as a
potential active component of the operating system itself.

The goal is not simply to build another operating system. The goal is to
understand the layers that make modern computing possible, identify where
existing designs create limitations, and experiment with alternative
architectures.

---

## Current Status

ORIGIN OS is in its early **bring-up and exploration phase**.

The project has already reached its first complete boot path:

```text
UEFI Firmware
      ↓
OriginBoot
      ↓
Load kernel.elf
      ↓
Parse ELF
      ↓
Load kernel segments
      ↓
Transfer control
      ↓
ORIGIN OS Kernel
      ↓
"Welcome to ORIGIN OS"
```

The current implementation successfully boots an x86-64 freestanding
kernel through a custom UEFI bootloader in QEMU using OVMF.

This is intentionally a small foundation. Most operating-system
subsystems have not been implemented yet.

---

## What Exists Today

### OriginBoot

`bootloader/` contains the custom UEFI bootloader for ORIGIN OS.

OriginBoot currently:

- Runs as a UEFI application using gnu-efi
- Locates the filesystem containing the bootloader
- Opens `kernel.elf`
- Reads the kernel into memory
- Validates the ELF image
- Parses ELF program headers
- Loads `PT_LOAD` segments
- Obtains the kernel entry point
- Transfers execution to the kernel

See [`bootloader/README.md`](bootloader/README.md) for bootloader-specific
documentation.

### Initial Kernel

`kernel/` contains the first ORIGIN OS kernel implementation.

The current kernel is intentionally minimal. It is a freestanding x86-64
kernel with an explicit linker script and entry point.

Its current job is simple:

```text
Welcome to ORIGIN OS
```

The message is written through the serial interface so the kernel can be
observed while running under QEMU.

### Build System

The repository has a top-level build system that coordinates the kernel,
bootloader, disk image, and QEMU workflow.

From the repository root:

```bash
make
```

builds the system.

```bash
make run
```

builds the required components and launches QEMU.

For a clean rebuild:

```bash
make clean
make
```

Generated build artifacts are excluded from version control.

---

## Architecture Direction

ORIGIN OS is currently targeting **x86-64**.

The architecture is deliberately being developed incrementally rather
than being locked into a complete design before implementation begins.

Early work is therefore focused on understanding the fundamental
interfaces between:

```text
Hardware
   ↓
Firmware
   ↓
Bootloader
   ↓
Kernel
   ↓
Memory Management
   ↓
CPU Scheduling
   ↓
Drivers
   ↓
Filesystems
   ↓
Networking
   ↓
Security
   ↓
Higher-Level System Services
   ↓
AI / Adaptive Components
```

Architectural decisions will be documented as the project develops.

---

## Research Direction

The project is not only about implementing conventional OS components.

A major part of ORIGIN OS is investigating problems and trade-offs in
existing operating-system designs.

Current research topics include:

- Complexity
- Performance
- Scalability
- Reliability and fault handling
- Security and isolation
- Communication between system components
- Flexibility versus specialization

These documents are exploratory research notes rather than claims that a
single alternative architecture has already been proven.

---

## AI as a Systems Component

One of the long-term research directions of ORIGIN OS is the integration
of AI into the operating-system architecture itself.

The idea is not simply to ship a chatbot alongside a conventional OS.

The project explores whether machine-learning systems could eventually
participate directly in system-level tasks such as:

- Adaptive resource management
- System observation and anomaly detection
- Security analysis
- Device and workload characterization
- Scheduling decisions
- Performance optimization
- Fault detection and recovery
- System configuration and adaptation

This is a future research direction, not a component of the current
kernel.

The first priority is understanding and building the underlying systems
correctly.

---

## Development Philosophy

ORIGIN OS is being built from the bottom up.

The development process intentionally favors:

1. Understanding a subsystem
2. Implementing a minimal version
3. Testing it in isolation
4. Integrating it with the system
5. Documenting what was learned
6. Identifying its limitations
7. Iterating toward a more capable design

The project is intentionally not trying to jump directly from an empty
repository to a feature-complete operating system.

The first milestone is understanding the machine.

---

## Roadmap

The roadmap is intentionally flexible, but the broad progression is:

### Phase 1 — Boot and Kernel Bring-Up

- [x] UEFI bootloader skeleton
- [x] UEFI filesystem access
- [x] Kernel file loading
- [x] ELF parsing
- [x] ELF loadable-segment loading
- [x] Kernel entry-point handoff
- [x] Minimal x86-64 kernel
- [x] Automated build and QEMU workflow
- [ ] Robust boot error handling
- [ ] UEFI memory-map acquisition
- [ ] `ExitBootServices()`
- [ ] Kernel boot information structure

### Phase 2 — Kernel Foundations

- [ ] CPU initialization
- [ ] Interrupt and exception handling
- [ ] Kernel console / logging
- [ ] Physical memory management
- [ ] Virtual memory
- [ ] Paging
- [ ] Kernel heap
- [ ] Kernel panic and diagnostics
- [ ] Basic process/thread model
- [ ] System-call mechanism

### Phase 3 — Core OS Subsystems

- [ ] Scheduler
- [ ] Device model
- [ ] Driver framework
- [ ] Filesystem layer
- [ ] Storage drivers
- [ ] Input devices
- [ ] Networking stack
- [ ] Inter-process communication

### Phase 4 — Security and Isolation

- [ ] User/kernel privilege boundaries
- [ ] Process isolation
- [ ] Memory protection
- [ ] Capability or permission model
- [ ] Secure boot-chain considerations
- [ ] Device isolation
- [ ] Security monitoring and auditing

### Phase 5 — Adaptive Systems

- [ ] System telemetry
- [ ] Resource behavior analysis
- [ ] Anomaly detection research
- [ ] Adaptive scheduling experiments
- [ ] Adaptive resource management
- [ ] AI-assisted security research
- [ ] AI-assisted system optimization

These phases are not a promise of a fixed final architecture. They are
a working direction for exploration.

---

## Building

### Requirements

The current development environment requires tools including:

- GCC
- GNU binutils
- gnu-efi
- mtools
- dosfstools
- QEMU
- OVMF

### Build

From the repository root:

```bash
make
```

### Run

```bash
make run
```

### Clean

```bash
make clean
```

The current development workflow is primarily intended for QEMU/OVMF
testing while the kernel and bootloader are being brought up.

---

## Project Status

ORIGIN OS should currently be considered:

**Experimental / Educational / Research**

It is not a production operating system and should not be used for
critical workloads or data.

At this stage, the most important output of the project is not the number
of features implemented. It is the understanding gained by implementing
and testing the system layer by layer.

---

## Long-Term Vision

The long-term goal is to evolve ORIGIN OS from a minimal bootable kernel
into a complete experimental operating system with its own architectural
ideas around:

- Computing
- Memory
- Processes
- Security
- Devices
- Networking
- Adaptation
- Intelligence

The project will remain grounded in traditional systems engineering while
experimenting with ideas that may challenge conventional OS boundaries.

For now, however, it starts with something much simpler:

```text
Firmware
   ↓
Bootloader
   ↓
Kernel
   ↓
Hello, ORIGIN OS.
```

And from there, we build upward.