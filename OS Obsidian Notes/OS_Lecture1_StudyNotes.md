# 🖥️ Operating Systems – CS x61
## Lecture 1: Introduction 
> **Textbook:** Operating Systems: Internals and Design Principles, 9th Ed. — William Stallings (Pearson, 2018)

---

## 📋 Course Overview

### Aims
- Study principles underlying the **design & implementation** of contemporary operating systems
- Understand the **major components** of an OS
- Introduction to design, **concurrent processing**, synchronization, and storage management
- Learn **current trends** in Operating Systems

### Course Outline
| # | Topic |
|---|-------|
| 1 | Introduction ← **(This Lecture)** |
| 2 | Operating Systems Structure |
| 3 | Processes and Threads |
| 4 | Concurrency Management |
| 5 | Memory Management Techniques |
| 6 | Process Scheduling |
| 7 | Disk Scheduling |
| 8 | Device Management |
| 9 | File Systems |
| 10 | Protection & Security |
| 11 | Virtual Machines |
| 12 | Case Studies: UNIX, Linux, Windows, Android |

### Grading
| Component | Weight |
|-----------|--------|
| Final Exam | 50 marks |
| Year Work (Midterm + Sheets + Lab + Project) | 50 marks |
| Lecture | Monday 2nd lecture: 10:20–12:00 |
| Minimum Attendance | 75% |
| Teaching Assistant | Eng. Sajed El Morsy |

### Textbooks
| | Book | Details |
|--|------|---------|
| ✅ | **Main:** Operating Systems: Internals and Design Principles | Stallings, Pearson, 9th Ed. 2018, ISBN: 978-0134670959 |
| 📖 | **Ref 1:** Modern Operating Systems | Tanenbaum, Pearson, 4th Ed. 2014, ISBN: 978-1292061429 |
| 📖 | **Ref 2:** Operating System Concepts | Silberschatz, Galvin & Gagne, Wiley, 9th Ed. 2012, ISBN: 978-1118063330 |

---

## 💻 What is an Operating System?

An OS is system software that:
- **Exploits hardware resources** of one or more processors to provide services to system users
- **Manages secondary memory** and I/O devices
- **Provides support** for networking and communications

> 💡 The OS acts as an intermediary between user applications and the physical hardware.

---

## 🔧 Basic Hardware Elements

Four fundamental components connected via the **System Bus**:

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│  PROCESSOR  │     │ MAIN MEMORY │     │ I/O MODULES │
│   (CPU)     │     │   (RAM)     │     │             │
└──────┬──────┘     └──────┬──────┘     └──────┬──────┘
       │                   │                   │
       └───────────────────┴───────────────────┘
                           │
                  ┌────────┴────────┐
                  │   SYSTEM BUS    │
                  │ (Communication) │
                  └─────────────────┘
```

### 1. Processor (CPU)
- **Controls** the operation of the entire computer
- **Performs** all data processing functions
- Also called the **Central Processing Unit (CPU)**

#### Processor Registers
Registers are **faster and smaller** than main memory.

| Register | Type | Description |
|----------|------|-------------|
| **Data Registers** | User-Visible | Hold data values; changeable by user |
| **Address Registers** | User-Visible | Hold memory addresses (Index, Segment Pointer, Stack Pointer) |
| **PC** (Program Counter) | Control/Status | Address of the **next** instruction to fetch |
| **IR** (Instruction Register) | Control/Status | Holds the **most recently fetched** instruction |
| **MAR** (Memory Address Reg) | Control/Status | Address in memory for a read/write |
| **MBR** (Memory Buffer Reg) | Control/Status | Data being written to / read from memory |
| **PSW** (Program Status Word) | Control/Status | Condition codes + interrupt info + processor mode |
| **Condition Codes** | Control/Status | Results of operations (zero, negative, overflow…) |

#### Program Status Word (PSW) — Pentium EFLAGS
Every processor design includes a PSW register containing condition codes + status info.

| Flag | Meaning | Flag | Meaning |
|------|---------|------|---------|
| CF | Carry Flag | DF | Direction Flag |
| PF | Parity Flag | IF | Interrupt Enable Flag |
| AF | Auxiliary Carry Flag | TF | Trap Flag |
| ZF | Zero Flag | SF | Sign Flag |
| OF | Overflow Flag | NT | Nested Task Flag |
| IOPL | I/O Privilege Level | RF | Resume Flag |
| VM | Virtual 8086 Mode | AC | Alignment Check |
| VIF | Virtual Interrupt Flag | VIP | Virtual Interrupt Pending |
| ID | Identification Flag | | |

### 2. Main Memory
- Stores **data and programs** currently in use
- Typically **volatile** — contents lost when power is off
- Also called **real memory** or **primary memory (RAM)**

### 3. I/O Modules
Move data between the computer and the external environment:
- Secondary memory devices (e.g., hard disks, SSDs)
- Communications equipment (network cards)
- Terminals and human-interface devices

### 4. System Bus
- Provides **communication pathways** among all components
- Connects processor ↔ main memory ↔ I/O modules

---

## ⚙️ Microprocessor & Multiprocessors

### Microprocessor
- Contains an **entire processor on a single chip**
- The invention that enabled desktop and handheld computing
- Fastest general-purpose processors available

### Multiprocessors / Multi-core
- Each chip (socket) contains **multiple processors (cores)**
- **Multicore:** multiple computing cores on a **single chip**
- On-chip communication is **faster** than between-chip communication
- One multicore chip uses **significantly less power** than multiple single-core chips

> 💡 Example: Intel Core i7-5960X — 8 cores, each with 32kB L1 + 256kB L2 cache, plus a shared 20MB L3 cache.

---

## ▶️ Instruction Execution

A program = a set of instructions stored in memory. The processor runs a continuous **fetch-execute cycle**.

```
       START
         │
         ▼
┌─────────────────┐
│   FETCH STAGE   │  ← PC points to next instruction → loaded into IR
│  Fetch Next     │  ← PC is incremented after each fetch
│  Instruction    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  EXECUTE STAGE  │  ← IR is decoded and executed
│  Execute        │
│  Instruction    │
└────────┬────────┘
         │
         ▼
    HALT (or loop back to FETCH)
```

### How It Works Step by Step
1. **PC** holds address of the next instruction
2. Processor **fetches** instruction from memory at that address
3. **PC is incremented** after each fetch
4. Fetched instruction is loaded into **IR**
5. IR is **decoded** → instruction is **executed**

### Four Types of Instructions
| Type | Description |
|------|-------------|
| **Processor–Memory** | Transfer data between processor and memory |
| **Processor–I/O** | Transfer data to or from a peripheral device |
| **Data Processing** | Arithmetic or logical operations on data |
| **Control** | Alter sequence of execution (e.g., jump, branch) |

---

## ⚡ Interrupts

### Why Interrupts?
Without interrupts, the CPU would waste time waiting for slow I/O devices.

> 💡 Example: A 1 GHz CPU executes ~10⁹ instructions/sec. A 7200 RPM hard disk has a half-track time of 4ms — making it **4,000,000× slower** than the CPU. Without interrupts, the CPU sits idle waiting.

### What is an Interrupt?
- A mechanism by which hardware/software can **interrupt the normal sequencing** of the processor
- When an interrupt occurs, control transfers to the **Interrupt Service Routine (ISR)**
- The ISR determines the **nature of the interrupt** and performs the necessary actions

### Interrupt Cycle

```
       START
         │
         ▼
┌─────────────────┐     ┌──────────────────────────────┐
│   FETCH STAGE   │     │      INTERRUPT STAGE         │
│  Fetch next     │     │  Check for pending interrupt  │
│  instruction    │     │  at the END of each cycle     │
└────────┬────────┘     └──────────────────────────────┘
         │
         ▼
┌─────────────────┐
│  EXECUTE STAGE  │──→ Interrupts DISABLED → back to FETCH
│  Execute        │──→ Interrupts ENABLED  → INTERRUPT STAGE
│  instruction    │
└────────┬────────┘
         │
         ▼
       HALT

• No interrupt pending → fetch next instruction (continue)
• Interrupt pending   → suspend current program → run ISR
```

### Simple Interrupt Processing — Step by Step

**HARDWARE side:**
1. Device controller or hardware **issues an interrupt**
2. Processor **finishes executing** the current instruction
3. Processor **signals acknowledgment** of the interrupt
4. Processor **pushes PSW and PC** onto the control stack
5. Processor **loads new PC** value based on interrupt type

**SOFTWARE side (ISR runs):**
1. **Save** remainder of process state information
2. **Process** the interrupt (handle the event)
3. **Restore** process state information
4. **Restore** old PSW and PC → resume original program

### Multiple Interrupts — Two Solutions

| Solution | How It Works | Pros & Cons |
|----------|-------------|-------------|
| **1. Disable Interrupts** | During ISR, all interrupts disabled. They stay pending until ISR completes. | ✅ Simple | ❌ Strict sequential order, no priority |
| **2. Priority Scheme (Nested)** | Higher-priority interrupts can interrupt lower-priority ISRs | ✅ Respects urgency | ✅ More realistic |

### Nested Interrupts Example
Three I/O devices:

| Device | Priority |
|--------|----------|
| Communications Line | 5 (Highest) |
| Disk | 4 |
| Printer | 2 (Lowest) |

```
t=0:  User Program running
t=10: Printer interrupt (priority 2) → enters Printer ISR
t=15: Comm line interrupt (priority 5 > 2) → interrupts Printer ISR → enters Comm ISR
t=25: Disk interrupt (priority 4) arrives; Comm ISR finishes → enters Disk ISR
t=35: Disk ISR completes → returns to Printer ISR
t=40: Printer ISR completes → returns to User Program
```

### Classes of Interrupts
| Class | Triggered By |
|-------|-------------|
| **Program** | Instruction errors: arithmetic overflow, division by zero, illegal instruction, bad memory access |
| **Timer** | Processor-internal timer (used for time-sharing and scheduling) |
| **I/O** | I/O controller: signals normal completion OR error condition |
| **Hardware Failure** | Physical failure: power loss, memory parity error |

---

## 🗄️ Storage-Device Hierarchy

As you go **DOWN** the hierarchy:

```
┌──────────────────────────────────────┐  ← FASTEST | MOST EXPENSIVE | SMALLEST
│             REGISTERS                │     (on-chip, nanoseconds, bytes)
├──────────────────────────────────────┤
│               CACHE                  │     (L1/L2/L3, nanoseconds, KB–MB)
├──────────────────────────────────────┤
│         MAIN MEMORY (RAM)            │     (microseconds, GB)
├──────────────────────────────────────┤
│       SOLID-STATE DISK (SSD)         │     (microseconds–ms, TB)
├──────────────────────────────────────┤
│          HARD DISK (HDD)             │     (milliseconds, TB)
├──────────────────────────────────────┤
│           OPTICAL DISK               │     (slow, TB)
├──────────────────────────────────────┤
│          MAGNETIC TAPES              │     (very slow, TB–PB)
└──────────────────────────────────────┘  ← SLOWEST | CHEAPEST | LARGEST
```

| Property | Going Down the Hierarchy |
|----------|--------------------------|
| Cost per bit | **Decreasing** (cheaper) |
| Capacity | **Increasing** (more storage) |
| Access Time | **Increasing** (slower) |
| Volatility | **Decreasing** (more permanent) |
| CPU Access Frequency | **Decreasing** (accessed less often) |

> 💡 **Key trade-off:** Design the system so that lower (slower) levels are accessed as infrequently as possible. This is achieved through **caching**.

---

## ⚡ Caching

An important principle applied at many levels (hardware, OS, software).

### How Caching Works
- **Frequently-used data** is copied from slower storage into faster storage (the cache)
- When data is needed, the **cache is checked FIRST**
- **Cache HIT:** data found in cache → used directly ✅ (fast!)
- **Cache MISS:** data not in cache → fetched from main memory, loaded into cache
- Cache is **smaller** than the storage being cached
- Key design problems: **cache size** and **replacement policy**

### Cache Read Operation (Flowchart)

```
CPU sends Read Address (RA)
          │
          ▼
   Is block containing RA in cache?
          │
    YES ──┤── NO
     │         │
     ▼         ▼
Fetch RA    Access main memory for block containing RA
word from       │
cache       Allocate cache slot for the block
  │             │
  │         Load main memory block into cache slot
  │             │
  └─────────────┘
          │
          ▼
    Deliver RA word to CPU → DONE
```

### Cache Organization

**Single cache:**
```
CPU ←──(word)──→ Cache ←──(block)──→ Main Memory
```

**Three-level cache (modern processors):**
```
CPU ←→ L1 Cache ←→ L2 Cache ←→ L3 Cache ←→ Main Memory
       (Fastest)    (Fast)    (Less fast)    (Slow)

L1: Smallest & fastest (e.g., 32 kB per core)
L2: Larger, slightly slower (e.g., 256 kB per core)
L3: Largest, shared among all cores (e.g., 20 MB shared)
```

---

## 📡 I/O Structure & Techniques

When the processor encounters an I/O instruction, it issues a command to the appropriate I/O module. **Three techniques:**

### 1. Programmed I/O

```
CPU → Issues Read command to I/O module
CPU → Reads status of I/O module  (I/O → CPU)
         │
    NOT READY → Check status again (busy loop!) 🔄
         │
      READY → Read word from I/O Module (I/O → CPU)
           → Write word into memory (CPU → memory)
           → Done? NO → loop back
           → Done? YES → Next instruction ✅
```

- Processor **repeatedly polls** (checks) the I/O status register
- Processor responsible for moving data to/from main memory
- ❌ **Disadvantage:** Keeps processor busy needlessly (**busy-waiting**)

### 2. Interrupt-Driven I/O

```
CPU → Issues Read command to I/O module
CPU → Does something else (useful work!) ✅
         ... time passes ...
I/O Module → Sends INTERRUPT to CPU when ready
CPU → Reads status → reads word from I/O (I/O → CPU)
    → Writes word to memory (CPU → memory)
    → Done? NO → CPU does other work again
    → Done? YES → Next instruction ✅
```

- Processor issues command, then **continues doing useful work**
- Interrupted when I/O module is ready to exchange data
- ✅ **Advantage:** More efficient — no needless busy-waiting
- ❌ **Disadvantage:** Every **single word** read/written still passes through the processor — high overhead for large data

### 3. Direct Memory Access (DMA)

```
CPU → Issues Read Block command to DMA (CPU → DMA)
CPU → Does something else (completely free!) ✅
         ... DMA handles ENTIRE block transfer ...
DMA → Transfers block directly between device and memory
DMA → Sends ONE interrupt to CPU when entire block is done
CPU → Next instruction ✅ (data already in memory)
```

- A **separate DMA module** handles the transfer
- Processor specifies: read/write, device address, memory location, number of words
- **Only ONE interrupt** per entire block (not per byte)
- Processor involved **only at start and end**
- ✅ **Best for:** Large volumes of data (disk reads, video, network)

### I/O Techniques Comparison

| Feature | Programmed I/O | Interrupt-Driven I/O | DMA |
|---------|---------------|---------------------|-----|
| CPU involvement | Constant (polling) | Per word/byte | Start & end only |
| Interrupts per transfer | None | One per word | One per block |
| CPU efficiency | ❌ Very Low | ⚠️ Medium | ✅ High |
| Best for | Simple, slow I/O | Moderate I/O | High-speed bulk data |

---

## 🖥️ Computer System Architecture

### Single-Processor Systems
- Use a **single general-purpose processor**
- May also have special-purpose processors (disk controller, keyboard controller, GPU, etc.)

### Multiprocessor Systems
Also called **parallel systems** or **multicore systems**. Two or more processors sharing bus, memory, and/or peripherals.

### Advantages of Multiprocessors
| Advantage | Explanation |
|-----------|-------------|
| **Increased Throughput** | More work done per unit time (multiple processors working simultaneously) |
| **Economy of Scale** | Share peripherals, storage, power → cheaper than multiple separate systems |
| **Increased Reliability** | Graceful degradation: failure of one processor doesn't halt the system |
| **Scaling** | Vendors can offer a product range based on number of processors |

> ⚠️ Speed-up with N processors is **less than N** due to overhead and resource contention.

### Two Types of Multiprocessing

| Type | Description |
|------|-------------|
| **Asymmetric Multiprocessing (AMP)** | Each processor has a specific task. **Boss–worker** relationship: master schedules and allocates work to workers. |
| **Symmetric Multiprocessing (SMP)** | All processors are **equal** and can perform **all tasks**. More flexible and widely used. |

### SMP Characteristics
- Two or more similar processors of **comparable capability**
- Processors share the **same main memory**, interconnected by a bus
- Processors **share I/O devices**
- All processors can **perform the same functions**
- Controlled by an **integrated OS** managing interaction between processors

```
┌──────────────┐  ┌──────────────┐  ┌──────────────┐
│    CPU₀      │  │    CPU₁      │  │    CPU₂      │
│  [registers] │  │  [registers] │  │  [registers] │
│  [cache]     │  │  [cache]     │  │  [cache]     │
└──────┬───────┘  └──────┬───────┘  └──────┬───────┘
       │                  │                  │
       └──────────────────┴──────────────────┘
                          │
               ┌──────────┴──────────┐
               │    SHARED MEMORY    │
               └─────────────────────┘
```

### SMP Advantages
| Benefit | Detail |
|---------|--------|
| **Performance** | Multiple processes run simultaneously, each on a different processor |
| **Availability** | Failure of one process/processor does not halt the system |
| **Incremental Growth** | Add more processors to boost performance |
| **Scaling** | Vendors offer product ranges based on processor count |

### Multicore Design
- Multiple computing cores on a **single chip**
- On-chip communication is **faster** than between-chip
- **Significantly less power** than multiple single-core chips
- Example: Intel Core i7-5960X — 8 cores, L1+L2 per core, shared 20MB L3

---

## 🌐 Clustered Systems

Multiple complete systems (**nodes**) joined together.

### Characteristics
- Each node = single processor or multicore system
- Closely linked via **LAN or InfiniBand**
- Share storage via a **Storage-Area Network (SAN)**
- Primary goal: **high-availability** service that survives node failures

### Types of Clustering

| Type | Description |
|------|-------------|
| **Asymmetric Clustering** | One machine in **hot-standby** mode. If active server fails, standby takes over automatically. |
| **Symmetric Clustering** | Multiple nodes **all running applications AND monitoring each other**. More efficient (uses all hardware). |
| **HPC Cluster** | Runs applications **concurrently on ALL nodes**. Applications must be written for parallelization. |

### How High Availability Works
- Cluster software runs on all nodes
- Each node **monitors** one or more other nodes over the LAN
- If a monitored machine fails → monitoring machine **takes ownership of its storage** and restarts its applications
- A level of **redundancy** prevents single points of failure

---

## 📌 Quick Reference Cheat Sheet

### Key Terms
| Term | Definition |
|------|-----------|
| **Operating System** | Software managing hardware resources and providing services to users |
| **CPU / Processor** | Controls operations and processes data |
| **Program Counter (PC)** | Register holding address of the NEXT instruction |
| **Instruction Register (IR)** | Register holding the currently fetched instruction |
| **PSW** | Program Status Word — condition codes + processor status flags |
| **Interrupt** | Signal causing CPU to stop current task and run an ISR |
| **ISR** | Interrupt Service Routine — code that handles a specific interrupt |
| **Cache** | Fast, small memory storing frequently-accessed data |
| **Cache Hit** | Requested data IS found in the cache |
| **Cache Miss** | Requested data NOT in cache; must go to slower memory |
| **DMA** | Direct Memory Access — transfers data without CPU involvement |
| **SMP** | Symmetric Multiprocessing — all processors are equal |
| **AMP** | Asymmetric Multiprocessing — one boss, multiple workers |
| **Cluster** | Multiple systems joined for high-availability |
| **Multicore** | Single chip with multiple processor cores |
| **Volatile Memory** | Lost when power is removed (e.g., RAM) |
| **Non-Volatile Memory** | Retained without power (e.g., hard disk) |

### Interrupt Classes Summary
| Class | Triggered By |
|-------|-------------|
| Program | Instruction errors (overflow, div-by-zero, illegal instruction, bad memory access) |
| Timer | Processor-internal timer (time-sharing, scheduling) |
| I/O | I/O controller (completion or error) |
| Hardware Failure | Physical failure (power loss, memory parity error) |

### Storage Hierarchy — At a Glance
| Level | Speed | Size | Volatile? |
|-------|-------|------|-----------|
| Registers | ⚡⚡⚡⚡ Fastest | Bytes | Yes |
| Cache (L1/L2/L3) | ⚡⚡⚡ Very Fast | KB – MB | Yes |
| Main Memory (RAM) | ⚡⚡ Fast | GB | Yes |
| SSD | ⚡ Medium | TB | No |
| Hard Disk (HDD) | 🐢 Slow | TB | No |
| Optical Disk | 🐢🐢 Very Slow | GB–TB | No |
| Magnetic Tape | 🐢🐢🐢 Slowest | TB–PB | No |

---

## 🎯 Exam Tips

- Know the **4 basic hardware elements**: Processor, Main Memory, I/O Modules, System Bus
- Be able to explain the **fetch-execute cycle** step by step
- Know the role of **PC, IR, MAR, MBR, PSW** registers
- Know **all 4 classes of interrupts** and what triggers each
- Compare and contrast **Programmed I/O vs Interrupt-Driven I/O vs DMA**
- Memorize the **storage hierarchy order** and properties going down
- Explain **cache hit vs cache miss** and the cache read flowchart
- Know the difference between **SMP and AMP**
- Know the **advantages of multiprocessor systems**
- Explain how **clustered systems** provide high availability
- Understand **nested interrupts** and priority schemes

---

*CS x61 Operating Systems — Dr. Noha Adly*
