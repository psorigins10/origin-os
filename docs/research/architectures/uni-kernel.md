# Unikernel

A unikernel is a specialized system image that combines an application with only the OS functionality and libraries it needs.

Unlike a general-purpose OS, a unikernel is typically designed to run a single application or workload.

## Structure

              Unikernel
          ┌─────────────────┐
          │ Application      │
          │ OS components    │
          │ Libraries        │
          │ Required drivers │
          └────────┬────────┘
                   ↓
                Hardware

## Why Use It?

The goal is to create a small, specialized execution environment.

Removing unnecessary OS functionality can reduce resource usage and potentially reduce the attack surface.

## How Does It Work?

The application and required OS components are compiled into a single specialized image.

That image can run directly on hardware or inside a virtual machine/hypervisor.

## What Does It Solve?

- Reduces unnecessary OS functionality
- Creates specialized environments
- Can provide strong isolation between workloads
- Can reduce resource overhead
- Can enable fast startup

## Trade-offs

Specialization reduces flexibility.

Updating an application or dependency commonly requires building and deploying a new image.

Managing many different applications can therefore increase build, testing, deployment, and maintenance complexity.

## Key Trade-off

General-purpose OS
→ Flexibility + many applications
→ Larger software stack

Unikernel
→ Specialization + small footprint
→ Less flexibility + image management complexity

## Security Implications

A smaller software stack can reduce the attack surface.

When deployed as separate isolated instances, failure or compromise of one workload can potentially be contained from others.

However, a small image does not automatically make a system secure.

## Performance Implications

A unikernel can reduce overhead by removing unnecessary OS components and specializing the software stack for its workload.

It can also provide fast startup, which is useful for certain cloud and service workloads.

## Complexity Implications

The runtime environment can be simple and small.

However, complexity can move into the build and deployment system, especially when many applications require different unikernel images.

## Example Use Cases

- Cloud services
- Network appliances
- Dedicated servers
- Embedded systems
- Specialized machines

## Questions for Further Research

- How are unikernels isolated?
- How does a unikernel communicate with hardware?
- How are updates handled safely?
- How does a unikernel compare with containers?
- Can unikernels dynamically adapt without rebuilding?
- Can the benefits of unikernels be achieved without their deployment complexity?

## What Could ORIGIN Learn?

- Specialization can reduce unnecessary system overhead.
- Smaller software stacks can reduce the attack surface.
- Isolation can prevent one workload from affecting others.
- Specialization introduces maintenance and deployment challenges.
- ORIGIN could investigate whether an OS can achieve specialization dynamically instead of requiring a separate image for every workload.