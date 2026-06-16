# Lecture 2 - Operating System Principles
## Complete Exam Review Summary

---

## 1. What is an Operating System?

### Definition
- **A program that controls the execution of application programs**
- **An interface between applications and hardware**

### The "Beautiful vs Ugly Interface" Concept (⚠️ TRICKY - Exam Favorite)
```
┌─────────────────────────┐
│   Application programs  │  ← Beautiful interface (easy to use)
│    👑 🏰 🦚 🌷           │
├─────────────────────────┤
│    Operating System     │  ← The mediator
├─────────────────────────┤
│    😤 🐗 😱 🐸           │  ← Ugly interface (hardware is complex)
├─────────────────────────┤
│        Hardware         │
└─────────────────────────┘
```
**Key Point:** The OS hides the complexity of hardware from applications and users.

---

## 2. OS Objectives

| Objective | Description |
|-----------|-------------|
| **Convenience** | Makes the computer more convenient to use |
| **Efficiency** | Allows system resources to be used efficiently |
| **Ability to Evolve** | Permits development/testing of new functions without interfering with service |

---

## 3. System Layers & Views

### Different Perspectives
```
        End User ──→ Application Programs
                      ↑
        Programmer ──→ Utilities
                      ↑
   OS Designer ──→ Operating System
                      ↑
                  Computer Hardware
```

### Hardware & Software Infrastructure (⚠️ IMPORTANT DIAGRAM)
```
┌─────────────────────────────────────────┐
│        Application programs             │
│  ┌─────────────────────────────────┐    │
│  │      Libraries/utilities        │    │ Software
│  │  ┌─────────────────────────┐    │    │
│  │  │    Operating system     │    │    │
│  │  │                         │    │    │
│──┴──┴─────────────────────────┴────┴────┤
│           Execution hardware            │
│  ┌─────────────────┐  ┌─────────────┐   │
│  │ System intercon-│  │   Memory    │   │ Hardware
│  │ nect (bus)      │  │ translation │   │
│──┴─────────────────┴──┴─────────────┴───┤
│  I/O devices and    │    Main memory      │
│     networking      │                     │
└─────────────────────────────────────────┘

Interfaces:
• Application Programming Interface (API)
• Application Binary Interface (ABI)
• Instruction Set Architecture (ISA)
```

---

## 4. Operating System Services

### Core Services
1. **Program Development** - Editors, debuggers, frameworks
2. **Program Execution** - Loading, initialization, resource scheduling
3. **Access to I/O Devices** - Uniform interface, hides hardware details
4. **Controlled Access to Files** - Authorization, sharing, protection, caching
5. **System Access** - Protection, authorization, conflict resolution
6. **Error Detection and Response** - Hardware errors (memory, device failure) + Software errors (arithmetic, access violations)
7. **Accounting** - Statistics for performance monitoring and billing

---

## 5. Basic OS Organization

### The Four Managers (⚠️ MUST KNOW)
```
┌─────────────────────────────────────────────┐
│           Operating System                  │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  │
│  │ Process  │──│  File    │  │ Device   │  │
│  │ & Resource│  │ Manager  │  │ Manager  │  │
│  │ Manager  │──┤          │  │          │  │
│  │    │     │  │          │  │          │  │
│  └────┼─────┘  └────┬─────┘  └────┬─────┘  │
│       │             │             │         │
│       └─────────────┼─────────────┘         │
│                     │                       │
│              ┌──────────┐                   │
│              │ Memory   │                   │
│              │ Manager  │                   │
│              └────┬─────┘                   │
└───────────────────┼─────────────────────────┘
                    │
┌───────────────────┼─────────────────────────┐
│  Hardware Layer   │                         │
│  ┌─────────┐ ┌───┴────┐ ┌─────────┐       │
│  │Processor│ │ Main   │ │ Devices │       │
│  │  (s)    │ │ Memory │ │         │       │
│  └─────────┘ └────────┘ └─────────┘       │
└─────────────────────────────────────────────┘
```

**Key Relationships:**
- Process & Resource Manager coordinates ALL other managers
- Each manager controls a specific hardware resource

---

## 6. OS as Resource Manager

### Key Concepts
- OS executes as ordinary software but with special intent
- **Directs processor in use of resources**
- **Relinquishes control** to execute other programs
- Built around **Kernel/Nucleus** - portion in main memory with most-frequently used functions

### Memory Layout
```
┌─────────────────────────────┐
│    Operating System Software │ ← Kernel (resident in memory)
├─────────────────────────────┤
│     Programs and Data       │ ← User programs
└─────────────────────────────┘
        ↓
┌─────────────────────────────┐
│  Storage (Disk)             │
│  ┌─────┐                    │
│  │  OS  │ ← Full OS stored  │
│  ├─────┤                    │
│  │Programs                   │
│  ├─────┤                    │
│  │ Data │                    │
│  └─────┘                    │
└─────────────────────────────┘
```

---

## 7. Modes of Operation (⚠️ EXAM TRICKY)

| Aspect | User Mode | Kernel Mode (Monitor Mode) |
|--------|-----------|---------------------------|
| **What runs** | User programs | OS/Monitor |
| **Memory access** | Certain areas PROTECTED | ALL memory accessible |
| **Instructions** | Certain instructions NOT allowed | ALL instructions allowed (privileged) |
| **Purpose** | Protection from user errors | Full system control |

**Why two modes?** Memory protection and privileged instructions require it!

---

## 8. Multiprogramming vs Uniprogramming (⚠️ CRITICAL - EXAM HEAVY)

### Uniprogramming (BAD - CPU idle)
```
Program A: [Run]----[Wait I/O]----[Run]----[Wait I/O]----
                    ↑ CPU IDLE!   ↑ CPU IDLE!
```
**Problem:** Processor must wait for I/O to complete before proceeding

### Multiprogramming (GOOD - CPU busy)
```
Program A: [Run]----------[Wait]----------[Run]----------[Wait]
Program B:      [Wait][Run]----------[Wait]----------[Run]----
Program C:           [Wait][Run]----------[Wait]----------[Run]
Combined:  [RunA][RunB][RunC]--[Wait]--[RunA][RunB][RunC]-[Wait]
                    ↑ CPU ALWAYS BUSY!
```

**How it works:**
1. Multiple processes reside in main memory simultaneously
2. OS picks and executes one job
3. When job blocks on I/O → CPU switches to another job
4. CPU is NEVER idle as long as there are ready jobs

---

## 9. Multiprogramming Example (⚠️ KNOW THE NUMBERS)

### Job Characteristics
| Job | Type | Duration | Memory | Disk | Terminal | Printer |
|-----|------|----------|--------|------|----------|---------|
| JOB1 | Heavy compute | 5 min | 50M | No | No | No |
| JOB2 | Heavy I/O | 15 min | 100M | No | Yes | No |
| JOB3 | Heavy I/O | 10 min | 75M | Yes | No | Yes |

### Performance Comparison
| Metric | Uniprogramming | Multiprogramming |
|--------|---------------|------------------|
| Processor use | 22% | **43%** |
| Memory use | 30% | **67%** |
| Disk use | 33% | **67%** |
| Printer use | 33% | **67%** |
| Elapsed time | 30 min | **15 min** |
| Throughput rate | 6 jobs/hr | **12 jobs/hr** |
| Avg response time | 18 min | **10 min** |

**Key Takeaway:** Multiprogramming nearly doubles resource utilization!

---

## 10. Time Sharing (⚠️ DISTINGUISH FROM MULTIPROGRAMMING)

### Definition
- Extension of multiprogramming for **interactive jobs**
- Processor time shared between multiple users
- Uses **time slice/quantum** (e.g., 0.2 seconds in CTSS)
- Current user is **preempted** after each time slice

### Batch Multiprogramming vs Time Sharing (⚠️ EXAM TABLE)

| Aspect | Batch Multiprogramming | Time Sharing |
|--------|----------------------|--------------|
| **Principal objective** | Maximize processor use | **Minimize response time** |
| **Source of directives** | Job control language commands | **Commands entered at terminal** |
| **User interaction** | None (batch) | Interactive |
| **Response time** | Not critical | **Critical** |

**CTSS (1962):** 32K words, 0.2 sec switching, up to 32 users

---

## 11. Problems with Time Sharing
1. **Memory protection** - Jobs must not interfere with each other
2. **File system protection** - Only authorized users can access
3. **Resource contention** - Printers, storage, etc.

---

## 12. Major OS Achievements

1. **Processes**
2. **Memory management**
3. **Information protection and security**
4. **Scheduling and resource management**
5. **System structure**

---

## 13. Processes (⚠️ FUNDAMENTAL CONCEPT)

### Definitions (ALL are correct!)
- A **program in execution**
- An **instance of a running program**
- The entity that can be assigned to and executed on a **processor**
- A unit of activity with a **single sequential thread**, current state, and associated system resources

### Process Components (3 parts)
```
┌─────────────────────────────────────────────────────────────┐
│                      Main Memory                            │
│  ┌─────────┐                                                │
│  │Process  │                                                │
│  │  List   │──→ i (index for Process A)                     │
│  │    i    │──→ j (index for Process B)                     │
│  │    j    │                                                │
│  └────┬────┘                                                │
│       │                                                     │
│       │    ┌─────────────────┐                              │
│       └───→│   Process A     │                              │
│            │  ┌───────────┐  │                              │
│            │  │  Context  │  │ ← Execution context/state    │
│            │  ├───────────┤  │                              │
│            │  │   Data    │  │ ← Variables, workspace       │
│            │  ├───────────┤  │                              │
│            │  │ Program   │  │ ← Executable code            │
│            │  │  (code)   │  │                              │
│            │  └───────────┘  │                              │
│            └─────────────────┘                              │
│                                                             │
│            ┌─────────────────┐                              │
│            │   Process B     │                              │
│            │  ┌───────────┐  │                              │
│            │  │  Context  │  │                              │
│            │  ├───────────┤  │                              │
│            │  │   Data    │  │                              │
│            │  ├───────────┤  │                              │
│            │  │ Program   │  │                              │
│            │  │  (code)   │  │                              │
│            │  └───────────┘  │                              │
│            └─────────────────┘                              │
└─────────────────────────────────────────────────────────────┘
```

### Execution Context Contains (⚠️ KNOW THIS)
- **Program Counter (PC)** - points to next instruction
- **Base & Limit registers** - define memory region
- **Other registers** - general purpose
- **Priority** - scheduling priority
- **I/O wait status** - waiting for which I/O event

### Context Switch (⚠️ EXAM DEFINITION)
- OS saves state of current process
- Loads state of new process
- Process index register points to current running process

---

## 14. Process Coordination Problems (⚠️ TRICKY - 4 TYPES)

| Problem | Description | Example |
|---------|-------------|---------|
| **Improper synchronization** | Signals lost or duplicated | Missing wake-up signals |
| **Failed mutual exclusion** | Two processes use shared resource simultaneously | Two users editing same file |
| **Non-determinate operation** | Programs overwrite common memory | Shared memory corruption |
| **Deadlocks** | Two+ programs hang waiting for each other | A waits for B, B waits for A |

---

## 15. Memory Management (5 Responsibilities)

1. **Process isolation** - Prevents interference among programs
2. **Automatic allocation and management** - Transparent to user
3. **Support for modular programming** - Dynamic process creation/elimination
4. **Protection and access control** - For sharing
5. **Long-term storage** - Persistent data

### Solutions
- **Virtual memory** - Logical addressing without physical memory limits
- **File system** - Named objects for long-term storage

---

## 16. Paging & Virtual Memory (⚠️ IMPORTANT)

### Paging
- Process divided into fixed-size blocks called **pages**
- Virtual address = **page number + offset**
- MMU provides dynamic mapping between virtual and real addresses
- Page can be located **anywhere** in main memory
- If page not in memory → **page fault** → loaded from disk

### Virtual Memory Addressing
```
Processor → Virtual Address → [MMU] → Real Address → Main Memory
                                    ↓
                              Disk Address ← Secondary Memory
```

**Key:** Address translation hardware (MMU) sits between processor and memory

---

## 17. Data Protection and Security (4 Aspects)

| Aspect | Definition |
|--------|-----------|
| **Availability** | Protecting against interruption |
| **Confidentiality** | Users cannot read unauthorized data |
| **Data integrity** | Protection from unauthorized modification |
| **Authenticity** | Proper verification of user identity and data validity |

---

## 18. Scheduling and Resource Management (3 Factors)

| Factor | Description |
|--------|-------------|
| **Fairness** | Equal and fair access to resources |
| **Differential responsiveness** | Discriminate among different job classes |
| **Efficiency** | Maximize throughput, minimize response time |

### Three Queue Types (⚠️ KNOW THE DIFFERENCE)

```
New Jobs → [Long-term Queue] → OS allocates memory → [Short-term Queue] → CPU executes
                                                          ↓
                                                   [I/O Queue] → Device
```

| Queue | Purpose | Managed By |
|-------|---------|------------|
| **Long-term queue** | New jobs waiting for processor | OS allocates memory, moves to short-term |
| **Short-term queue** | Processes in memory, ready to run | Short-term scheduler (RR, priority) |
| **I/O queue** | Processes waiting for I/O device | OS decides which process gets device |

**⚠️ TRICKY:** OS must NOT over-commit memory or CPU!

---

## 19. System Structure

### Layered Architecture
- Each level performs related subset of functions
- Each layer provides services to layer above
- Each layer relies on functions from layer below
- **Change in one layer should not affect others**
- Lower levels deal with shorter time scales

### Hierarchical OS Model (Levels 1-7)
| Level | Name | Objects | Operations |
|-------|------|---------|------------|
| 7 | Virtual Memory | Segments, pages | Read, write, fetch |
| 6 | Local secondary store | Blocks, device channels | Read, write, allocate, free |
| 5 | Primitive processes | Primitive process, semaphores | Suspend, resume, wait, signal |
| 4 | Interrupts | Interrupt-handling programs | Invoke, mask, unmask, retry |
| 3 | Procedures | Procedures, call stack | Mark stack, call, return |
| 2 | Instruction Set | Evaluation stack, scalar/array data | Load, store, add, branch |
| 1 | Electronic circuits | Registers, buses | Clear, transfer, activate |

---

## 20. Modern OS Developments

### Four Key Developments
1. **Microkernel architecture**
2. **Multithreading**
3. **Symmetric multiprocessing (SMP)**
4. **Distributed OS**
5. **Object-oriented design**

---

## 21. Multithreading (⚠️ THREAD vs PROCESS)

### Comparison
```
┌─────────────────────┐    ┌─────────────────────────────────────┐
│       THREAD        │    │              PROCESS                │
├─────────────────────┤    ├─────────────────────────────────────┤
│ • Dispatchable unit │    │ • Collection of one or more threads │
│   of work           │    │ • Associated system resources         │
│                     │    │   (memory, code, data, files)       │
│ • Processor context │    │                                     │
│   (PC, Stack Ptr)   │    │ • Breaking app into threads gives   │
│ • Own data area for │    │   greater control over modularity   │
│   stack             │    │   and timing                        │
│                     │    │                                     │
│ • Executes          │    │                                     │
│   sequentially      │    │                                     │
│ • Interruptible     │    │                                     │
└─────────────────────┘    └─────────────────────────────────────┘
```

**Key Point:** A process is a container; threads are the units of execution within it.

---

## 22. Multiprogramming vs Multiprocessing vs Multithreading (⚠️ EXAM TRICKY)

```
MULTIPROGRAMMING (1 CPU):
P1: [Run][Blocked][Run][Blocked][Run]
P2: [Blocked][Run][Blocked][Run][Blocked]
P3: [Blocked][Blocked][Run][Blocked][Run]
     ↑ Only ONE process runs at any instant (interleaving)

MULTIPROCESSING (2+ CPUs):
P1: [Run][Blocked][Run][Blocked][Run]
P2: [Run][Blocked][Run][Blocked][Blocked]
P3: [Blocked][Run][Blocked][Run][Blocked]
     ↑ Multiple processes can RUN SIMULTANEOUSLY (overlapping)

MULTITHREADING:
- Single process divided into multiple threads
- Threads share process resources
- Can run concurrently (on SMP) or interleaved (on uniprocessor)
```

**Relationships:**
- Multithreading useful even on **uniprocessor**
- SMP useful even for **non-threaded processes**
- They **complement** each other

---

## 23. Microkernel Architecture (⚠️ EXAM HEAVY)

### Monolithic Kernel (OLD)
```
┌─────────────┐
│   Users     │
├─────────────┤
│ File System │
├─────────────┤
│     IPC     │
├─────────────┤
│ I/O & Device│
├─────────────┤
│Virtual Mem  │
├─────────────┤
│Primitive PM │
├─────────────┤
│  HARDWARE   │
└─────────────┘
```
**Problem:** Large, complex, everything in kernel mode

### Microkernel (MODERN)
```
User Mode:  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐
            │ Client  │ │ Device  │ │  File   │ │ Process │ │Virtual│
            │Processes│ │ Drivers │ │ Server  │ │ Server  │ │Memory │
            └────┬────┘ └────┬────┘ └────┬────┘ └────┬────┘ └───┬───┘
                 │           │           │           │          │
                 └───────────┴───────────┴───────────┴──────────┘
                                     │
Kernel Mode:                    ┌─────────┐
                                │Microkernel│
                                └────┬────┘
                                     │
                                   HARDWARE
```

### Microkernel Benefits (⚠️ KNOW ALL 7)
1. **Uniform interface** - All services via message passing
2. **Extensibility** - Add new services easily
3. **Flexibility** - Add/subtract features
4. **Portability** - Only microkernel needs changes for new processor
5. **Reliability** - Modular design = rigorous testing
6. **Distributed system support** - Local and remote servers treated same
7. **Object-oriented** - Easier customization

### Kernel vs User Mode Services
| Kernel Mode | User Mode (Servers) |
|-------------|---------------------|
| Essential core functions | Device drivers |
| IPC | File systems |
| Basic scheduling | Virtual memory manager |
| | Windowing system |
| | Security services |

---

## 24. Virtual Machines & Virtualization

### Definition
- Enables single PC/server to run **multiple OSs** simultaneously
- Host OS supports multiple **Virtual Machines (VMs)**
- Each VM has characteristics of particular OS and hardware platform

### Types
| Type | Description | Speed |
|------|-------------|-------|
| **Emulation** | Source CPU ≠ Target CPU | SLOWEST |
| **Interpretation** | Language not compiled to native code | Slow |
| **Virtualization** | OS natively compiled, guest also native | FAST |

### VMM (Virtual Machine Manager)
- Provides virtualization services
- Can run **natively** (no general-purpose host) - e.g., VMware ESX, Citrix XenServer

### Virtualization Diagram
```
Without VM:              With VM:
┌──────────┐            ┌──────────┐ ┌──────────┐ ┌──────────┐
│ processes│            │ processes│ │ processes│ │ processes│
└────┬─────┘            └────┬─────┘ └────┬─────┘ └────┬─────┘
     │                       │            │            │
┌────┴─────┐            ┌────┴────┐ ┌───┴────┐ ┌───┴────┐
│  kernel  │            │  kernel │ │ kernel │ │ kernel │
└────┬─────┘            └───┬─────┘ └───┬────┘ └───┬────┘
     │                      │           │          │
┌────┴─────┐            ┌───┴────┐    VM1      VM2      VM3
│ hardware │            │  VMM   │
└──────────┘            └───┬─────┘
                            │
                         hardware
```

---

## 25. Cloud Computing

### Definition
Delivers computing, storage, apps as service across network

### Types
| Type | Description | Example |
|------|-------------|---------|
| **Public cloud** | Available via Internet to anyone | AWS, Azure |
| **Private cloud** | Run by company for own use | Internal data center |
| **Hybrid cloud** | Both public and private components | Mixed deployment |

### Service Models (⚠️ KNOW THE ACRONYMS)
| Model | Full Name | Description | Example |
|-------|-----------|-------------|---------|
| **SaaS** | Software as a Service | Applications via Internet | Word processor, payroll |
| **PaaS** | Platform as a Service | Software stack ready for use | Database server |
| **IaaS** | Infrastructure as a Service | Servers/storage over Internet | Backup storage |

### Cloud Architecture
```
Internet → Firewall → Load Balancer → [Virtual Machines, Storage, Cloud Management]
                ↑
        Customer requests via Cloud Customer Interface
```

---

## 26. Fault Tolerance

### Definition
Ability of system to continue normal operation despite hardware/software faults

### Key Measures (⚠️ FORMULAS)
- **Reliability R(t)** - Probability of correct operation up to time t
- **MTTF** (Mean Time To Failure) - Average time between failures
- **MTTR** (Mean Time To Repair) - Average time to repair/replace
- **Availability** - Fraction of time system is available

```
        Up
         │    ┌──────────┐    ┌──────────┐    ┌──────────┐
         │    │    B1    │    │    B2    │    │    B3    │
         │    │ (working)│    │ (working)│    │ (working)│
         │    └──────────┘    └──────────┘    └──────────┘
Down ────┴────┘          └────┘          └────┘
              ←─A1─→     ←─A2─→         ←─A3─→
              (repair)   (repair)       (repair)

MTTF = (B1 + B2 + B3) / 3
MTTR = (A1 + A2 + A3) / 3
Availability = MTTF / (MTTF + MTTR)
```

### Availability Classes (⚠️ MEMORIZE)
| Class | Availability | Annual Downtime |
|-------|-------------|-----------------|
| Continuous | 1.0 | 0 |
| Fault Tolerant | 0.99999 | 5 minutes |
| Fault Resilient | 0.9999 | 53 minutes |
| High Availability | 0.999 | 8.3 hours |
| Normal Availability | 0.99 - 0.995 | 44-87 hours |

### Fault Categories
| Type | Description | Subtypes |
|------|-------------|----------|
| **Permanent** | Always present until repaired | - |
| **Temporary** | Not present all the time | Transient (occurs once), Intermittent (multiple times) |

### Redundancy Methods (3 types)
1. **Spatial (physical)** - Multiple components, one as backup
2. **Temporal** - Repeat function when error detected (good for temporary faults only!)
3. **Information** - Replicate/coding data for error detection/correction

---

## 27. Windows Architecture

### Structure
- **Modular, object-oriented design**
- Separation between **User Mode** and **Kernel Mode**

### Kernel Mode Components
```
┌─────────────────────────────────────────────────────────────┐
│ User Mode: Applications, Environment Subsystems (Win32, POSIX)│
│            Service Processes, System Support Processes        │
├─────────────────────────────────────────────────────────────┤
│ Ntdll.dll (Native API)                                      │
├─────────────────────────────────────────────────────────────┤
│ Kernel Mode:                                                  │
│ ┌─────────────────────────────────────────────────────────┐ │
│ │ System Service Dispatcher                               │ │
│ ├─────────┬─────────┬─────────┬─────────┬─────────────────┤ │
│ │I/O Mgr  │File Sys │Object   │Plug&Play│Power Mgr        │ │
│ │         │Cache    │Manager  │Manager  │Security Ref Mon │ │
│ ├─────────┴─────────┴─────────┴─────────┴─────────────────┤ │
│ │ Kernel (scheduling, process switching)                    │ │
│ ├─────────────────────────────────────────────────────────┤ │
│ │ Hardware Abstraction Layer (HAL)                          │ │
│ └─────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### Key Components
| Component | Function |
|-----------|----------|
| **Executive** | Core OS services: memory, process, security, I/O, IPC |
| **Kernel** | Controls processor execution, scheduling, switching |
| **HAL** | Isolates OS from platform-specific hardware |
| **Device Drivers** | Translate user I/O to hardware requests |
| **Windowing/Graphics** | GUI functions |

### User-Mode Processes
1. **Special system processes** - Session manager, authentication, logon
2. **Service processes** - Spooler, event logger, network services
3. **Environment subsystems** - Win32, POSIX personalities
4. **User applications** - EXEs and DLLs

### Windows Key Features
- Supports **threads** and **SMP**
- OS routines can run on any available processor
- Multiple threads in same process can execute on different processors
- Server processes use multiple threads for multiple clients

---

## 28. UNIX Architecture

### Structure (Concentric Circles)
```
        ┌─────────────────────────────┐
        │   UNIX Commands & Libraries   │
        │  ┌───────────────────────┐  │
        │  │  System Call Interface  │  │
        │  │  ┌─────────────────┐  │  │
        │  │  │     Kernel      │  │  │
        │  │  │  ┌─────────┐  │  │  │
        │  │  │  │ Hardware │  │  │  │
        │  │  │  └─────────┘  │  │  │
        │  │  └─────────────────┘  │  │
        │  └───────────────────────┘  │
        │  User-written Applications   │
        └─────────────────────────────┘
```

### Traditional UNIX Kernel
```
User Programs → Trap/Libraries → System Call Interface
                                       ↓
                    ┌──────────────────┼──────────────────┐
                    │                  │                  │
              File Subsystem ←────→ Process Control Subsystem
                    │                  │                  │
              Buffer Cache      Interprocess     Scheduler
                    │           Communication    Memory Mgmt
                    ↓                  │
              Device Drivers           │
              (Character/Block)        │
                    │                  │
                    └──────┬───────────┘
                           ↓
                    Hardware Control
                           ↓
                         Hardware
```

### UNIX Family Tree (⚠️ KNOW RELATIONSHIPS)
```
Research Unix (Bell Labs, 1970)
    │
    ├──→ BSD Family ──→ BSD 4.4 ──→ FreeBSD, NetBSD, OpenBSD
    │                    └──→ SunOS 4.1.4
    │                    └──→ NextStep 3.3 ──→ OS X (macOS)
    │
    ├──→ Xenix OS
    │
    ├──→ GNU ──→ Linux 4.7
    │
    └──→ System V Family ──→ Commercial Unix ──→ UnixWare
                              ├──→ Solaris 11.3
                              ├──→ HP-UX 11iv3
                              └──→ AIX 7.2
```

### System V Release 4 (SVR4) Features
- Real-time processing support
- Process scheduling classes
- Dynamically allocated data structures
- Virtual memory management
- Virtual file system
- **Preemptive kernel**

### BSD Variants
| Variant | Characteristics |
|---------|----------------|
| **FreeBSD** | Internet servers, firewalls, embedded systems |
| **NetBSD** | Many platforms, embedded systems |
| **OpenBSD** | Special emphasis on **security** |

### Solaris 11
- Oracle's SVR4-based UNIX
- Fully preemptable, multithreaded kernel
- Full SMP support
- Object-oriented file system interface

---

## 29. Linux

### History
- Created by **Linus Torvalds** (Finnish student)
- First posted on Internet in **1991**
- Free, open source, source code available
- Key to success: availability of free software packages

### Architecture: Modular Monolithic Kernel
- **NOT microkernel** but achieves similar advantages
- Collection of modules loaded/unloaded on demand
- All OS functionality in one large block running as single process

### Loadable Modules (⚠️ 2 KEY CHARACTERISTICS)
1. **Dynamic linking** - Load/unlink while kernel executes, saves memory
2. **Stackable modules** - Hierarchical with dependencies, common code in single module

### Linux Kernel Components
```
User Level:     Processes
                    ↓
                ┌───────┐
Kernel Level:   │Signals│←──────┐
                └───┬───┘        │
                    ↓            │
                ┌─────────┐      │
                │System   │←─────┘
                │Calls    │
                └───┬─────┘
        ┌───────────┼───────────┐
        ↓           ↓           ↓
   ┌────────┐  ┌────────┐  ┌──────────┐
   │Process │  │ File   │  │ Network  │
   │& Sched │  │Systems │  │ Protocols│
   └───┬────┘  └───┬────┘  └────┬─────┘
       ↓           ↓            ↓
   ┌────────┐  ┌────────┐  ┌──────────┐
   │Virtual │  │Char    │  │ Block    │
   │Memory  │  │Device  │  │ Device   │
   └───┬────┘  │Drivers │  │ Drivers  │
       ↓       └────┬───┘  └────┬─────┘
   ┌────────┐       │           │
   │Physical│       └─────┬─────┘
   │Memory  │             ↓
   └────────┘        ┌─────────┐
                     │Interrupts│
                     └────┬────┘
Hardware Level:  CPU  System   Terminal  Disk   Network
                     Memory                    Interface
```

---

## 30. Android Operating System

### Overview
- Linux-based, originally for mobile phones
- Most popular mobile OS
- Developed by Android Inc., bought by Google in **2005**
- First commercial version: **Android 1.0 (2008)**
- Open-source nature = key to success

### Architecture Layers
```
┌─────────────────────────────────────────────┐
│           APPLICATIONS                      │
│  Home, Dialer, SMS, Browser, Camera, etc.   │
├─────────────────────────────────────────────┤
│        APPLICATION FRAMEWORK                  │
│  Activity Manager  │  Content Providers        │
│  Window Manager  │  View System              │
│  Package Manager  │  Notification Manager     │
│  Telephony Mgr  │  Location Manager         │
│  Resource Mgr   │  XMPP Service               │
├─────────────────────────────────────────────┤
│  SYSTEM LIBRARIES  │    ANDROID RUNTIME       │
│  Surface Manager   │    ┌─────────────┐       │
│  OpenGL/ES         │    │Core Libraries│      │
│  Media Framework   │    ├─────────────┤       │
│  SQLite            │    │Dalvik/ART VM │      │
│  LibWebCore        │    └─────────────┘       │
│  Libc              │                            │
├─────────────────────────────────────────────┤
│            LINUX KERNEL                       │
│  Display, Camera, Bluetooth, Flash, Binder  │
│  USB, Keypad, WiFi, Audio, Power Mgmt       │
└─────────────────────────────────────────────┘
```

### Implementation Languages
- **Applications & Framework:** Java
- **System Libraries:** C/C++
- **Linux Kernel:** C

### Application Framework Components (⚠️ KNOW FUNCTIONS)
| Component | Function |
|-----------|----------|
| **Activity Manager** | Manages lifecycle of applications (start, stop, resume) |
| **Window Manager** | Java abstraction of Surface Manager, manages client area |
| **Package Manager** | Installs and removes applications |
| **Telephony Manager** | Phone, SMS, MMS services |
| **Content Providers** | Share data between apps (e.g., contacts) |
| **Resource Manager** | Manages localized strings, bitmaps |
| **View System** | UI primitives and events |
| **Location Manager** | GPS, cell tower, WiFi databases |
| **Notification Manager** | Messages, appointments |
| **XMPP** | Standardized messaging between applications |

### Alternative Android Architecture View
```
Applications and Framework
           ↓
      Binder IPC
           ↓
┌─────────────────────────────────────┐
│      Android System Services        │
│  ┌─────────────┐ ┌──────────────┐ │
│  │ Media Server │ │ System Server│ │
│  │ • AudioFlinger│ │ • Power Mgr │ │
│  │ • MediaPlayer│ │ • Window Mgr │ │
│  │ • Camera     │ │ • Activity   │ │
│  │ • Other Media│ │   Manager    │ │
│  └─────────────┘ │ • Other Svcs │ │
│                   └──────────────┘ │
└─────────────────────────────────────┘
           ↓
    Android Runtime/Dalvik
           ↓
┌─────────────────────────────────────┐
│   Hardware Abstraction Layer (HAL)  │
│  Camera HAL │ Audio HAL │ Graphics │
└─────────────────────────────────────┘
           ↓
       Linux Kernel
```

---

## ⚠️ TRICKY CONCEPTS & EXAM TRAPS

### 1. Multiprogramming vs Multiprocessing vs Multithreading
| Concept | CPUs | What Runs | Key Feature |
|---------|------|-----------|-------------|
| Uniprogramming | 1 | 1 program | CPU idle during I/O |
| Multiprogramming | 1 | Multiple programs | CPU switches on I/O block |
| Time Sharing | 1 | Multiple interactive | Preemption by time quantum |
| Multiprocessing | 2+ | Multiple processes | TRUE parallel execution |
| Multithreading | 1+ | Multiple threads in process | Shared process resources |

### 2. Kernel vs User Mode
- **Kernel mode:** ALL instructions, ALL memory, OS code
- **User mode:** Restricted instructions, protected memory, user code
- **Trap/Interrupt:** Mechanism to switch from user to kernel mode

### 3. Process vs Thread
- **Process:** Resource container (code, data, files, memory)
- **Thread:** Unit of execution (PC, stack, registers)
- **One process** can have **many threads**
- Threads within same process **share memory**

### 4. Monolithic vs Microkernel
| Feature | Monolithic | Microkernel |
|---------|-----------|-------------|
| Size | Large | Small |
| Services in kernel | All | Only essential |
| Other services | In kernel | User-mode servers |
| Performance | Faster (no message passing) | More overhead |
| Reliability | Less (one bug crashes all) | More (isolated servers) |
| Extensibility | Hard | Easy |

### 5. Virtual Memory vs Physical Memory
- **Physical memory:** Actual RAM hardware
- **Virtual memory:** Logical address space per process
- **MMU:** Hardware that translates virtual → physical
- **Page fault:** Virtual address not in physical memory → load from disk

### 6. Fault Tolerance Metrics
- **MTTF:** Mean Time To Failure (want HIGH)
- **MTTR:** Mean Time To Repair (want LOW)
- **Availability = MTTF / (MTTF + MTTR)**
- **5 nines (0.99999)** = fault tolerant = 5 min downtime/year

### 7. Three Queues (Don't Confuse!)
- **Long-term:** New jobs waiting to ENTER system
- **Short-term:** Ready jobs waiting for CPU
- **I/O:** Jobs waiting for specific I/O device

### 8. UNIX vs Linux vs Android
| OS | Type | Key Feature |
|----|------|-------------|
| UNIX | Commercial/Academic | Original, many variants |
| Linux | Open source | Modular monolithic, loadable modules |
| Android | Mobile OS | Linux kernel + Java framework + Dalvik/ART |

---

## 📊 Quick Reference Formulas

```
CPU Utilization = (Time CPU is busy) / (Total time)

Throughput = Number of jobs completed / Time period

Response Time = Time from submission to first response

Availability = MTTF / (MTTF + MTTR)

MTTF = Σ(Up times) / Number of failures

MTTR = Σ(Repair times) / Number of repairs
```

---

## 🎯 Exam Checklist

- [ ] OS definition and objectives
- [ ] System layers (user, programmer, OS designer views)
- [ ] OS services (all 7 categories)
- [ ] Four managers and their relationships
- [ ] Kernel vs user mode (differences, why needed)
- [ ] Multiprogramming concept and benefits
- [ ] Multiprogramming example numbers (22%→43%, 30min→15min)
- [ ] Time sharing vs batch multiprogramming table
- [ ] Process definition and components (3 parts)
- [ ] Execution context contents
- [ ] Context switch process
- [ ] Four process coordination problems
- [ ] Memory management responsibilities (5)
- [ ] Paging and virtual memory basics
- [ ] Data protection 4 aspects
- [ ] Scheduling 3 factors and 3 queue types
- [ ] Layered architecture principles
- [ ] Modern OS developments (5)
- [ ] Thread vs process differences
- [ ] Multiprogramming vs multiprocessing vs multithreading
- [ ] Microkernel benefits (7)
- [ ] Virtualization types (emulation, interpretation, virtualization)
- [ ] Cloud service models (SaaS, PaaS, IaaS)
- [ ] Fault tolerance metrics and formulas
- [ ] Availability classes table
- [ ] Redundancy types (3)
- [ ] Windows architecture (kernel mode components)
- [ ] UNIX structure and family tree
- [ ] Linux modular monolithic kernel
- [ ] Loadable module characteristics (2)
- [ ] Android architecture layers and components

---

*Generated for Obsidian - Use [[links]] for connected notes*
*Focus areas marked with ⚠️ - Review these first!*
