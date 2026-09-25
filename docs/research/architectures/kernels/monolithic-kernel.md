# Monolithic Kernel

A monolithic kernel is a kernel architecture where major operating-system services run with kernel privileges within a single kernel address space.

Examples of these services include process management, memory management, filesystems, networking, and device drivers.

## Failure Isolation

Because major components run with kernel privileges, a serious bug in one component can potentially affect the entire kernel or system.

This provides high performance and direct communication between components, but reduces fault isolation.

## Why Use a Monolithic Kernel?

A major reason is performance. Kernel components can communicate directly and share kernel resources without requiring communication across separate protection domains for every interaction.

The trade-off is weaker fault isolation: a failure in one privileged component can potentially affect the entire system.

## Types of kernals

Monolithic kernel
Microkernel
Hybrid kernel
Exokernel
Nanokernel
Unikernel
Multikernel