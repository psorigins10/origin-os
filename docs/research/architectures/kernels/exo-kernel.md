# Exokernel

An exokernel is a kernel architecture that minimizes high-level abstractions and gives applications more direct, controlled access to hardware resources.

The kernel mainly focuses on resource allocation and protection, while applications or library operating systems decide how to use those resources.

## Structure

              Exokernel
          ┌───────────────┐
          │ Protection     │
          │ Allocation     │
          │ Resource Mgmt  │
          └───────┬───────┘
                  │
          Raw/controlled
             resources
                  │
        ┌─────────┴─────────┐
        ↓                   ↓
   Library OS A        Library OS B
        ↓                   ↓
    Application          Application

## Why Use It?

The main goal is to give software greater control over hardware resources.

Applications can potentially optimize resource usage for their specific workloads instead of being forced to use fixed OS abstractions.

## Trade-offs

Giving applications more control also shifts more responsibility toward application-level software.

This can increase implementation complexity and make resource management and security more difficult.

## Key Trade-off

Traditional OS
→ Strong abstractions + easier application development
→ Less direct control

Exokernel
→ More hardware control + optimization potential
→ More responsibility + complexity

## Questions for Further Research

- How does an exokernel safely expose hardware resources?
- How are resources isolated between applications?
- What exactly is a library OS?
- How does an exokernel handle resource conflicts?
- What are the performance benefits?
- What security problems can arise from increased control?