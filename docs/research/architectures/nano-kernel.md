# Nanokernel

A nanokernel is a kernel architecture that keeps the privileged kernel layer extremely small, containing only the minimum mechanisms required to interact with hardware and enforce protection.

More operating system functionality can be moved outside the privileged kernel into user-space components.

## Structure

              Nanokernel
          ┌─────────────────┐
          │ Minimal Core     │
          │ Hardware control │
          │ Protection       │
          │ Interrupts       │
          └────────┬────────┘
                   │
          ┌────────┴────────┐
          ↓                 ↓
     OS Services       OS Services
     User Space        User Space
          ↓                 ↓
      Applications       Applications

## Why Use It?

The goal is to minimize the amount of code running with the highest privileges.

A smaller privileged core can reduce the trusted computing base (TCB) and make the kernel easier to reason about and potentially verify.

## Trade-offs

Moving more functionality outside the kernel can increase the complexity of the overall system.

User-space components also need mechanisms to communicate with the privileged core and access protected resources.

## Key Trade-off

Larger Kernel
→ More functionality in privileged space
→ Simpler communication + potentially more kernel complexity

Nanokernel
→ Minimal privileged core + smaller TCB
→ More functionality outside the kernel + greater system complexity

## Questions for Further Research

- What exactly belongs inside a nanokernel?
- How are interrupts handled?
- How is CPU time enforced?
- How are user-space services given hardware access?
- How does a nanokernel differ from a microkernel?
- Can a very small kernel still provide good performance?