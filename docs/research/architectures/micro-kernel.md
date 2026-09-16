# Microkernel

A microkernel is an OS architecture where the kernel keeps only a small set of fundamental mechanisms in privileged kernel space.

Services such as filesystems, device drivers, and networking can run as isolated user-space processes or servers.

## Structure

```text
                Microkernel
             ┌───────────────┐
             │ Core mechanisms│
             │ IPC            │
             │ Scheduling     │
             │ Memory/protection
             └───────┬───────┘
                     │
                    IPC
       ┌─────────────┼─────────────┐
       ↓             ↓             ↓
  Filesystem      Drivers       Network
   server          server        server
       │             │             │
       └────────── User Space ─────┘