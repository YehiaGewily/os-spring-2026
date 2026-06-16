# Operating Systems - Lecture 1: Introduction & Computer System Overview

> **Course:** CS x61 | **Instructor:** Dr. Noha Adly
> **Textbook:** William Stallings - *Operating Systems: Internals and Design Principles* (9th Ed.)

---

## 📋 Course Structure & Assessment

| Component | Weight |
|-----------|--------|
| Final Exam | 50% |
| Year Work (Midterm + Sheets + Lab + Project) | 50% |
| **Total** | **100%** |

- **Attendance Required:** 75%
- **Section TA:** Eng. Sajed El Morsy

### Course Outline (What to expect)
1. Introduction ← *You are here*
2. Operating Systems Structure
3. Processes and threads
4. Concurrency management
5. Memory Management techniques
6. Process Scheduling
7. Disk Scheduling
8. Device Management
9. File Systems
10. Protection & Security
11. Virtual machines
12. Case studies: UNIX, Linux, Windows, Android

---

## 🎯 What is an Operating System?

**Definition:** Software that:
- Exploits hardware resources of one or more processors
- Provides a set of services to system users
- Manages secondary memory and I/O devices
- Provides support for networking and communications

> 🚨 **TRICKY:** OS is NOT just about the CPU - it manages ALL hardware resources including memory, I/O, and networking.

---

## 🏗️ Basic Elements of a Computer System

```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│  Processor  │    │ Main Memory │    │ I/O Modules │
│    (CPU)    │◄──►│   (RAM)     │◄──►│             │
└──────┬──────┘    └──────┬──────┘    └──────┬──────┘
       │                  │                  │
       └──────────────────┼──────────────────┘
                          │
                   ┌─────────────┐
                   │  System Bus │  ← Provides communication among all components
                   └─────────────┘
```

### 1. Processor (CPU)
- **Controls** the operation of the computer
- **Performs** data processing functions
- Also called: Central Processing Unit (CPU)

### 2. Main Memory
- Stores data and programs
- **Typically VOLATILE** → contents lost when computer shuts down
- Also called: **Real Memory** or **Primary Memory**

> 🚨 **TRICKY:** Main memory is NOT secondary storage (disks). It's RAM - fast but volatile!

### 3. I/O Modules
- Move data between computer and external environment
- Examples:
  - Secondary memory devices (disks, SSDs)
  - Communications equipment (network cards)
  - Terminals (keyboard, mouse, display)

### 4. System Bus
- Communication pathway connecting processor, memory, and I/O

---

## 🧠 CPU Internal Structure (IMPORTANT FOR EXAM)

```
┌─────────────────────────────────────┐
│                CPU                  │
│  ┌─────┐    ┌─────┐               │
│  │ PC  │    │ MAR │◄──────────────┼──────► Main Memory
│  └─────┘    └─────┘               │         (Instructions & Data)
│  ┌─────┐    ┌─────┐               │
│  │ IR  │    │ MBR │◄──────────────┼──────►
│  └─────┘    └─────┘               │
│       ┌─────────┐                 │
│       │Execution│                 │
│       │  Unit   │                 │
│       └─────────┘                 │
│  ┌─────────┐    ┌─────────┐       │
│  │ I/O AR  │    │ I/O BR  │◄──────┼──────► I/O Module
│  └─────────┘    └─────────┘       │         (Buffers)
└─────────────────────────────────────┘
```

### Key Registers (KNOW THESE!)

| Register | Full Name | Function |
|----------|-----------|----------|
| **PC** | Program Counter | Address of **next instruction** to fetch |
| **IR** | Instruction Register | Most recently **fetched instruction** |
| **MAR** | Memory Address Register | Address of memory location to access |
| **MBR** | Memory Buffer Register | Data being transferred to/from memory |
| **I/O AR** | I/O Address Register | Address of I/O device |
| **I/O BR** | I/O Buffer Register | Data being transferred to/from I/O |

> 🚨 **TRICKY:** PC points to the NEXT instruction, not the current one! After fetch, PC is incremented.

---

## 📊 Processor Registers Categories

### User-Visible Registers
- Minimize main memory access by optimizing register use
- Referenced by machine language
- Available to ALL programs (application + system)

**Types:**
1. **Data Registers** - can be changed by user
2. **Address Registers** - memory addresses of data/instructions
   - Index registers
   - Segment pointers (used in memory addressing)
   - Stack Registers (user/supervisor stacks)

### Control & Status Registers
- May or may NOT be visible to user
- **Program Counter (PC)** - address of next instruction
- **Instruction Register (IR)** - most recently fetched instruction
- **Condition Codes** - results of operations
- **MAR/MBR** - memory reference registers
- **Program Status Word (PSW)** - condition codes + interrupts + mode

> 🚨 **TRICKY:** PSW contains MORE than just condition codes - it also has interrupt enable/disable bits and processor mode (user/supervisor).

---

## ⚡ Instruction Execution Cycle

```
     ┌─────────────────────────────────────┐
     │                                     │
     ▼                                     │
┌─────────────┐      ┌─────────────┐      │
│    START    │─────►│ Fetch Next  │──────┘
└─────────────┘      │ Instruction │
                     └──────┬──────┘
                            │
                            ▼
                     ┌─────────────┐
                     │   Execute   │
                     │ Instruction │
                     └──────┬──────┘
                            │
                            ▼
                     ┌─────────────┐
                     │    HALT     │
                     └─────────────┘
```

### Two Steps (Basic)
1. **Fetch:** Processor reads instruction from memory (where PC points)
2. **Execute:** Processor executes the instruction

### Detailed Flow:
1. PC points to instruction address
2. Fetch instruction → place in IR
3. Decode instruction in IR
4. Execute
5. PC incremented
6. Repeat

### Instruction Types (4 categories - KNOW THESE!)

| Type | Description | Example |
|------|-------------|---------|
| **Processor-Memory** | Transfer data between processor and memory | LOAD, STORE |
| **Processor-I/O** | Data transferred to/from peripheral | READ, WRITE |
| **Data Processing** | Arithmetic or logic operations | ADD, SUB, AND, OR |
| **Control** | Alter sequence of execution | JUMP, BRANCH, HALT |

> 🚨 **TRICKY:** Control instructions don't process data - they CHANGE the flow of execution (PC modification)!

---

## 🔔 INTERRUPTS (MAJOR EXAM TOPIC!)

### Why Interrupts?
- **Problem:** Most I/O devices are MUCH slower than processor
- **Example:** 1GHz PC ≈ 10⁹ instructions/second
  - Hard disk at 7200 RPM: half-track rotation = 4ms
  - **4 million times slower** than processor!
- **Without interrupts:** Processor must pause and wait → WASTEFUL

### What is an Interrupt?
Mechanism by which other modules may interrupt the normal sequencing of the processor.

```
User Program                    Interrupt Handler (ISR)
┌─────────┐                    ┌─────────┐
│    1    │                    │         │
├─────────┤                    ├─────────┤
│    2    │                    │         │
├─────────┤                    │         │
│   ...   │                    │   ...   │
├─────────┤ ──Interrupt────► │         │
│    i    │    occurs here     │         │
├─────────┤ ◄───────────────── │         │
│   i+1   │                    │         │
├─────────┤                    │         │
│   ...   │                    │         │
├─────────┤                    ├─────────┤
│    M    │                    │   END   │
└─────────┘                    └─────────┘
```

### Interrupt Handler (ISR - Interrupt Service Routine)
- Program that handles the interrupt
- Determines nature of interrupt
- Performs necessary actions
- Returns control to original program

---

## 🔄 Flow of Control Comparison

### (a) WITHOUT Interrupts (Programmed I/O)
```
Time ──────────────────────────────────────►

User Program:  [1]──[WRITE]──[2]──[WRITE]──[3]──[WRITE]
                    │            │            │
                    ▼            ▼            ▼
I/O Program:       [4]──[5]    [4]──[5]     [4]──[5]
                   END         END          END

Problem: CPU WAITING during I/O! (Black bars = idle time)
```

### (b) WITH Interrupts (Short I/O Wait)
```
Time ───────────────────────────────────────────────────────►

User Program:  [1]──[2a]✕──[2b]──[3a]✕──[3b]
                    │              │
                    ▼              ▼
I/O Handler:      [4]──[5]       [4]──[5]
                   END            END

✕ = interrupt occurs here
CPU does useful work during I/O! (No idle time)
```

> 🚨 **TRICKY:** Even with interrupts, if I/O wait is SHORT, there might not be enough time to do useful work!

---

## 🔄 Interrupt Cycle (Flowchart)

```
                    ┌─────────────────┐
                    │ Fetch Stage     │
                    │ Fetch next      │◄────────────────────────┐
                    │ instruction     │                         │
                    └────────┬────────┘                         │
                             │                                  │
                             ▼                                  │
                    ┌─────────────────┐     Interrupts        │
                    │ Execute Stage   │     Disabled          │
                    │ Execute         │─────────────────────────┘
                    │ instruction     │
                    └────────┬────────┘
                             │
              Interrupts     │     Interrupts
              Disabled       │     Enabled
                             ▼
                    ┌─────────────────┐
                    │ Interrupt Stage │
                    │ Check for       │
                    │ interrupt;      │
                    │ initiate        │
                    │ handler         │
                    └─────────────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │      HALT       │
                    └─────────────────┘
```

### Interrupt Cycle Steps:
1. At end of each instruction cycle, processor checks for interrupts
2. **No interrupt** → fetch next instruction for current program
3. **Interrupt pending** → suspend current program, execute interrupt handler

---

## 🔧 Simple Interrupt Processing (Hardware vs Software)

```
HARDWARE SIDE                    SOFTWARE SIDE
┌─────────────────────────┐     ┌─────────────────────────┐
│ Device controller or      │     │ Save remainder of       │
│ other system hardware     │     │ process state information │
│ issues an interrupt       │     │                         │
└───────────┬─────────────┘     └───────────┬─────────────┘
            │                                 │
            ▼                                 ▼
┌─────────────────────────┐     ┌─────────────────────────┐
│ Processor finishes        │     │ Process interrupt       │
│ execution of current    │     │                         │
│ instruction             │     │                         │
└───────────┬─────────────┘     └───────────┬─────────────┘
            │                                 │
            ▼                                 ▼
┌─────────────────────────┐     ┌─────────────────────────┐
│ Processor signals         │     │ Restore process state   │
│ acknowledgment of         │     │ information             │
│ interrupt               │     │                         │
└───────────┬─────────────┘     └───────────┬─────────────┘
            │                                 │
            ▼                                 ▼
┌─────────────────────────┐     ┌─────────────────────────┐
│ Processor pushes PSW    │     │ Restore old PSW and PC  │
│ and PC onto control     │     │                         │
│ stack                   │     │                         │
└───────────┬─────────────┘     └─────────────────────────┘
            │
            ▼
┌─────────────────────────┐
│ Processor loads new PC  │
│ value based on          │
│ interrupt               │
└─────────────────────────┘
```

### Key Actions:
1. **Hardware:**
   - Device signals interrupt
   - Processor finishes current instruction
   - Processor acknowledges interrupt
   - **Pushes PSW and PC onto control stack** ← CRITICAL!
   - Loads new PC (interrupt handler address)

2. **Software (Interrupt Handler):**
   - Save remaining process state (registers)
   - Process the interrupt
   - Restore process state
   - **Restore old PSW and PC** ← Return to original program!

> 🚨 **TRICKY:** The processor automatically saves PSW and PC, but the software handler must save ALL OTHER registers!

---

## 📥 Memory & Register Changes During Interrupt

```
BEFORE INTERRUPT (at location N)          AFTER INTERRUPT (Return)
┌──────────────────┐                       ┌──────────────────┐
│   Control Stack  │                       │   Control Stack  │
│      T-M         │◄── Stack Pointer      │     T-M          │
│       ...        │                       │     N+1          │◄── PC now points
│       T          │◄── Old Stack Ptr     │      T           │    to N+1 (next instr)
├──────────────────┤                       ├──────────────────┤
│                  │                       │                  │
│   Interrupt      │                       │   Interrupt      │
│   Service        │                       │   Service        │
│   Routine        │                       │   Routine        │
│   (Start at Y)   │                       │   (Start at Y)   │
│   (Return at Y+L)│                       │   (Return at Y+L)│
│                  │                       │                  │
├──────────────────┤                       ├──────────────────┤
│                  │                       │                  │
│   User's Program │                       │   User's Program │
│       N          │◄── PC was here       │       N          │
│      N+1         │                       │      N+1         │◄── Will execute next
│                  │                       │                  │
└──────────────────┘                       └──────────────────┘

Processor State:                            Processor State:
- PC = N+1 (next instruction)              - PC = N+1 (restored)
- Stack Pointer = T                          - Stack Pointer = T-M (restored)
- General Registers = current                - General Registers = restored
```

> 🚨 **TRICKY:** After interrupt, PC points to N+1 (the instruction AFTER where interrupt occurred), not N!

---

## 🎯 Multiple Interrupts

### Solution 1: Disable Interrupts During Processing
```
User Program ──► Handler X ──► Handler Y ──► Back to User
                    │              │
                    └──────────────┘
                    Sequential processing
```
- **Advantage:** Simple
- **Disadvantage:** No account for priority; strict sequential order

### Solution 2: Priority Scheme (Nested Interrupts)
```
User Program ──► Handler X ──► Handler Y ──► Back to X ──► Back to User
                    │              │
                    └──────────────┘
                    (Y interrupts X if Y > X priority)
```
- Lower-priority handler can be interrupted by higher-priority
- **Nested interrupts** supported

### Example: Nested Interrupts with Priorities

| Device | Priority |
|--------|----------|
| Printer | 2 (lowest) |
| Disk | 4 |
| Communication Line | 5 (highest) |

```
Timeline:
t=0:  User Program starts
t=10: Printer interrupts (priority 2) → Handler runs
t=15: Communication interrupts (priority 5) → INTERRUPTS Printer handler!
                  (Higher priority takes precedence)
t=25: Communication handler done → Return to Printer handler
t=25: Disk interrupts (priority 4) → INTERRUPTS Printer handler!
t=35: Disk handler done → Return to Printer handler
t=40: Printer handler done → Return to User Program
```

> 🚨 **TRICKY:** Higher number = Higher priority! Communication (5) > Disk (4) > Printer (2)

---

## 📋 Classes of Interrupts (TABLE - MEMORIZE!)

| Class | Cause | Examples |
|-------|-------|----------|
| **Program** | Condition from instruction execution | Arithmetic overflow, division by zero, illegal instruction, memory access violation |
| **Timer** | Timer within processor | OS performs functions on regular basis (scheduling) |
| **I/O** | I/O controller signal | Normal completion, error conditions |
| **Hardware Failure** | System failure | Power failure, memory parity error |

> 🚨 **TRICKY:** Timer interrupts are generated by the processor itself, NOT an external device! Used for OS scheduling.

---

## 💾 Storage-Device Hierarchy

```
        ┌─────────────┐
        │  Registers  │ ◄── Fastest, smallest, most expensive
        │   (CPU)     │     Most volatile
        └──────┬──────┘
               │
        ┌──────▼──────┐
        │    Cache    │ ◄── L1, L2, L3 (faster = closer to CPU)
        │  (SRAM)     │
        └──────┬──────┘
               │
        ┌──────▼──────┐
        │ Main Memory │ ◄── RAM (DRAM) - Volatile!
        │   (DRAM)    │
        └──────┬──────┘
               │
        ┌──────▼──────┐
        │     SSD     │ ◄── Solid State Disk (Flash)
        │  (Flash)    │
        └──────┬──────┘
               │
        ┌──────▼──────┐
        │  Hard Disk  │ ◄── Magnetic storage
        │  (HDD)      │
        └──────┬──────┘
               │
        ┌──────▼──────┐
        │ Optical Disk│ ◄── CD/DVD/Blu-ray
        │             │
        └──────┬──────┘
               │
        ┌──────▼──────┐
        │Magnetic Tape│ ◄── Slowest, largest, cheapest
        │             │     Least volatile (archival)
        └─────────────┘
```

### Trends (Going Down the Hierarchy):
- ⬇️ **Decreasing** cost per bit
- ⬆️ **Increasing** capacity
- ⬆️ **Increasing** access time
- ⬇️ **Decreasing** volatility (frequency of access)

> 🚨 **TRICKY:** Volatility decreases as you go DOWN (tapes keep data forever), but access time INCREASES!

---

## 🚀 Caching Principle

### Definition:
- Information in use copied from slower to faster storage **temporarily**
- Performed at many levels: hardware, OS, software

### How It Works:
```
CPU requests data at address RA
         │
         ▼
┌─────────────────┐
│ Is block with RA│──No──► Access main memory for block
│    in cache?    │         Allocate cache slot
└────────┬────────┘         Load block into cache
         │Yes                Deliver to CPU
         ▼
┌─────────────────┐
│ Fetch from cache│
│   (FAST!)       │
└─────────────────┘
```

### Cache Hit vs Miss:
- **Hit:** Data found in cache → use directly (FAST)
- **Miss:** Data not in cache → copy to cache from lower level, then use

### Cache Characteristics:
- Cache is **smaller** than storage being cached
- **Cache management** is important design problem:
  - Cache size
  - Replacement policy (what to evict when full)

### Cache Organization:

```
Single Cache:                    Three-Level Cache:
┌─────┐     Word Transfer       ┌─────┐    ┌─────┐    ┌─────┐
│ CPU │◄──►│Cache│◄──Block──►│Main │     │ CPU │◄──►│ L1  │◄──►│ L2  │◄──►│ L3  │◄──►│Main │
└─────┘  Fast  │     Transfer  │Mem  │     └─────┘ Fast │     │ Fast │ Less │     │ Slow │Mem  │
               └───────────────┴─────┘          └─────┘      └─────┘ fast └─────┘      └─────┘
```

> 🚨 **TRICKY:** CPU-cache transfer is WORD-by-WORD (small), but cache-memory transfer is BLOCK-by-BLOCK (larger)!

---

## 🔌 I/O Structure & Techniques

### Three I/O Techniques (KNOW THE DIFFERENCES!)

| Technique | CPU Involvement | Efficiency | Best For |
|-----------|----------------|------------|----------|
| **Programmed I/O** | High (busy waiting) | Low | Simple devices |
| **Interrupt-Driven I/O** | Medium (per word) | Medium | Moderate speed |
| **Direct Memory Access (DMA)** | Low (start/end only) | High | High-speed, large blocks |

---

### 1. Programmed I/O

```
┌─────────────────┐
│ Issue Read cmd  │──► CPU sends command to I/O
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Read status     │◄── CPU checks status (loop!)
└────────┬────────┘
         │
    ┌────▼────┐
    │ Ready?  │──No──┐ (loop back)
    └────┬────┘      │
         │Yes        │
         ▼           │
┌─────────────────┐  │
│ Read word from  │  │
│ I/O Module      │  │
└────────┬────────┘  │
         │           │
         ▼           │
┌─────────────────┐  │
│ Write word to   │  │
│ memory          │  │
└────────┬────────┘  │
         │           │
    ┌────▼────┐      │
    │  Done?  │──No──┘
    └────┬────┘
         │Yes
         ▼
┌─────────────────┐
│ Next Instruction│
└─────────────────┘
```

**Characteristics:**
- Processor sends request to I/O module
- I/O module performs action, sets status bits
- **Does NOT interrupt** processor when done
- Processor must **keep checking status** (polling/busy waiting)
- **Disadvantage:** Keeps processor busy needlessly!

> 🚨 **TRICKY:** In programmed I/O, the CPU does ALL the work - reading status, reading data, writing to memory. I/O module is "dumb."

---

### 2. Interrupt-Driven I/O

```
┌─────────────────┐
│ Issue Read cmd  │──► CPU sends command
└────────┬────────┘    └──► "Do something else"
         │                    (CPU is free!)
         │
         │◄────────────── Interrupt from I/O
         │                (when ready)
         ▼
┌─────────────────┐
│ Read status     │
└────────┬────────┘
    ┌────▼────┐
    │ Ready?  │──Error──► Handle error
    └────┬────┘
         │Yes
         ▼
┌─────────────────┐
│ Read word from  │
│ I/O Module      │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Write word to   │
│ memory          │
└────────┬────────┘
    ┌────▼────┐
    │  Done?  │──No──┐ (wait for next interrupt)
    └────┬────┘      │
         │Yes        │
         ▼           │
┌─────────────────┐  │
│ Next Instruction│◄─┘
└─────────────────┘
```

**Characteristics:**
- Processor issues I/O command, then does **other useful work**
- Processor **interrupted** when I/O module ready
- **More efficient** than programmed I/O
- **Disadvantage:** Every word read/written passes through processor
  - Consumes processor time for each word transfer

> 🚨 **TRICKY:** Interrupt-driven I/O frees the CPU between transfers, but STILL requires CPU to move every word. DMA solves this!

---

### 3. Direct Memory Access (DMA) ⭐ MOST EFFICIENT

```
CPU ──► DMA Controller ──► I/O Device
         │
         └── Direct transfer to/from Main Memory
             (NO CPU involvement during transfer!)

Flow:
1. CPU issues command to DMA:
   - Read or Write?
   - I/O device address
   - Memory starting location
   - Number of words to transfer

2. CPU goes away (does other work)

3. DMA transfers ENTIRE BLOCK directly between I/O and memory

4. DMA sends ONE interrupt to CPU when done
```

**Characteristics:**
- Used for **high-speed** I/O devices
- Device controller transfers **blocks** directly to/from main memory
- **Only ONE interrupt per block** (not per word!)
- Processor involved only at **beginning and end**
- Most efficient for **large volumes of data**

> 🚨 **TRICKY:** DMA bypasses the CPU for data transfer, but the CPU must still SET UP the DMA (give it the parameters). DMA doesn't decide what to do - it follows CPU's command.

---

## 🖥️ Computer System Architecture

### Single-Processor Systems
- One general-purpose processor
- May have special-purpose processors (disk controller, keyboard controller, graphics)
- Special-purpose processors do NOT make it multiprocessor!

> 🚨 **TRICKY:** A system with one CPU + GPU + disk controller is STILL single-processor! The GPU and disk controller are special-purpose, not general-purpose.

---

### Multiprocessor Systems (Parallel/Multicore)

**Definition:** Two or more processors in close communication
- Share computer bus
- Sometimes share clock, memory, peripheral devices

#### Advantages:
1. **Increased Throughput**
   - More processors = more throughput
   - Speed-up with N processors is **< N** (due to overhead + contention)

2. **Economy of Scale**
   - Cost less than multiple single-processor systems
   - Share peripherals, mass storage, power supplies

3. **Increased Reliability**
   - Graceful degradation
   - Fault tolerance

#### Two Types:

| Type | Description | Relationship |
|------|-------------|--------------|
| **Asymmetric** | Each processor assigned specific task | Boss-worker: boss schedules, workers execute |
| **Symmetric (SMP)** | Each processor performs all tasks | All equal, any processor can do anything |

---

## ⚖️ Symmetric Multiprocessor (SMP) - DETAILS

### Characteristics:
1. Two or more **similar processors** of comparable capability
2. Processors share **same main memory**
3. Interconnected by **bus or internal connection**
4. Share access to **I/O devices**
5. **All processors can perform same functions**
6. Controlled by **integrated OS** providing interaction at job, task, file, data levels

### SMP Architecture:
```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│    CPU₀     │    │    CPU₁     │    │    CPU₂     │
│  ┌───────┐  │    │  ┌───────┐  │    │  ┌───────┐  │
│  │Registers│  │    │  │Registers│  │    │  │Registers│  │
│  └───┬───┘  │    │  └───┬───┘  │    │  └───┬───┘  │
│  ┌───▼───┐  │    │  ┌───▼───┐  │    │  ┌───▼───┐  │
│  │ Cache │  │    │  │ Cache │  │    │  │ Cache │  │
│  └───┬───┘  │    │  └───┬───┘  │    │  └───┬───┘  │
└──────┼──────┘    └──────┼──────┘    └──────┼──────┘
       │                  │                  │
       └──────────────────┼──────────────────┘
                          │
                   ┌──────▼──────┐
                   │   Memory    │
                   │   (Shared)  │
                   └─────────────┘
```

### SMP Organization (Modern Chips):
```
┌─────────────┐  ┌─────────────┐       ┌─────────────┐
│    CHIP     │  │    CHIP     │  ...  │    CHIP     │
│ ┌─────────┐ │  │ ┌─────────┐ │       │ ┌─────────┐ │
│ │Processor│ │  │ │Processor│ │       │ │Processor│ │
│ ├─────────┤ │  │ ├─────────┤ │       │ ├─────────┤ │
│ │ L1 Cache│ │  │ │ L1 Cache│ │       │ │ L1 Cache│ │
│ │ L2 Cache│ │  │ │ L2 Cache│ │       │ │ L2 Cache│ │
│ └────┬────┘ │  │ └────┬────┘ │       │ └────┬────┘ │
└──────┼──────┘  └──────┼──────┘       └──────┼──────┘
       │                │                      │
       └────────────────┼──────────────────────┘
                        │
                 System Bus
                        │
       ┌────────────────┼────────────────┐
       │                │                │
  ┌────▼────┐     ┌─────▼─────┐    ┌────▼────┐
  │  Main   │     │  I/O      │    │  I/O    │
  │ Memory  │     │ Adapter   │    │ Adapter │
  └─────────┘     └───────────┘    └─────────┘
```

> 🚨 **TRICKY:** Each chip has its OWN L1 and L2 cache, but they all share main memory via system bus. Cache coherency is a major design challenge!

---

## 🔲 Multicore Design

### Dual-Core Example:
```
┌─────────────────────────────────────────┐
│              Single Chip                │
│  ┌─────────────┐    ┌─────────────┐   │
│  │  CPU core₀  │    │  CPU core₁  │   │
│  │ ┌─────────┐ │    │ ┌─────────┐ │   │
│  │ │Registers│ │    │ │Registers│ │   │
│  │ └───┬─────┘ │    │ └───┬─────┘ │   │
│  │ ┌───▼─────┐ │    │ ┌───▼─────┐ │   │
│  │ │  Cache  │ │    │ │  Cache  │ │   │
│  │ └───┬─────┘ │    │ └───┬─────┘ │   │
│  └─────┼───────┘    └─────┼───────┘   │
│        │                  │            │
│        └────────┬─────────┘            │
│                 │                      │
│            ┌────▼────┐                 │
│            │ Memory  │                 │
│            │ (Shared)│                 │
│            └─────────┘                 │
└─────────────────────────────────────────┘
```

### Why Multicore > Multiple Single-Core Chips:
1. **On-chip communication** faster than between-chip
2. **Less power** than multiple single-core chips
3. More efficient design

---

## 🧩 Intel Core i7-5960X Example

```
┌─────────┐ ┌─────────┐     ┌─────────┐ ┌─────────┐
│  Core 0 │ │  Core 1 │ ... │  Core 6 │ │  Core 7 │
│ 32kB    │ │ 32kB    │     │ 32kB    │ │ 32kB    │
│ L1-I    │ │ L1-I    │     │ L1-I    │ │ L1-I    │
│ 32kB    │ │ 32kB    │     │ 32kB    │ │ 32kB    │
│ L1-D    │ │ L1-D    │     │ L1-D    │ │ L1-D    │
│ 256kB   │ │ 256kB   │     │ 256kB   │ │ 256kB   │
│ L2      │ │ L2      │     │ L2      │ │ L2      │
└────┬────┘ └────┬────┘     └────┬────┘ └────┬────┘
     │           │               │           │
     └───────────┴───────────────┴───────────┘
                    │
            ┌───────▼────────┐
            │   20 MB L3     │ ◄── SHARED among all cores!
            │    Cache       │
            └───────┬────────┘
                    │
        ┌───────────┴───────────┐
        ▼                       ▼
┌───────────────┐       ┌───────────────┐
│ DDR4 Memory   │       │ PCI Express   │
│ Controllers   │       │ 40 lanes      │
│ 4×8B @ 2.133  │       │ @ 8 GT/s      │
│    GT/s       │       │               │
└───────────────┘       └───────────────┘
```

> 🚨 **TRICKY:** L1 is split into L1-I (instruction) and L1-D (data) - this is Harvard architecture at cache level! L2 is per-core, L3 is shared.

---

## ✅ SMP Advantages Summary

| Advantage | Description |
|-----------|-------------|
| **Performance** | More than one process can run simultaneously, each on different processor |
| **Availability** | Failure of single process does NOT halt the system |
| **Incremental Growth** | Performance enhanced by adding additional processor |
| **Scaling** | Vendors offer range of products based on number of processors |

---

## 🌐 Clustered Systems

### Definition:
- Multiprocessor systems with multiple **nodes** joined together
- Each node may be single processor or multicore
- Closely linked via **LAN** or **InfiniBand**
- Usually share storage via **Storage-Area Network (SAN)**

```
┌─────────┐   interconnect   ┌─────────┐   interconnect   ┌─────────┐
│Computer │◄───────────────►│Computer │◄───────────────►│Computer │
│  (Node) │                  │  (Node) │                  │  (Node) │
└────┬────┘                  └────┬────┘                  └────┬────┘
     │                            │                            │
     └────────────────────────────┼────────────────────────────┘
                                  │
                           ┌──────▼──────┐
                           │   Storage   │
                           │   Area      │
                           │   Network   │
                           │    (SAN)    │
                           └─────────────┘
```

### High-Availability Service:
- Layer of **cluster software** runs on nodes
- Each node monitors one or more others
- If monitored machine fails, monitoring machine:
  1. Takes ownership of its storage
  2. Restarts applications that were running

### Types of Clustering:

| Type | Description | Efficiency |
|------|-------------|------------|
| **Asymmetric** | One machine in **hot-standby** mode; others run applications | Low (standby does nothing) |
| **Symmetric** | Multiple nodes run applications AND monitor each other | High (uses all hardware) |

### High-Performance Computing (HPC):
- Applications written to use parallelization
- Run concurrently on ALL computers in cluster

> 🚨 **TRICKY:** Asymmetric clustering has a "hot-standby" that does NOTHING but monitor. Symmetric clustering is more efficient but requires more than one application to be available.

---

## 🎓 EXAM TRICKY QUESTIONS & CONCEPTS

### Q1: What's the difference between Programmed I/O and Interrupt-Driven I/O?
**A:** Programmed I/O = CPU polls/waits (busy waiting). Interrupt-Driven = CPU does other work, interrupted when ready. Both still require CPU to transfer every word!

### Q2: Why is DMA better than Interrupt-Driven I/O?
**A:** DMA transfers entire BLOCKS without CPU involvement. Only ONE interrupt per block vs. one per word. CPU only involved at start and end.

### Q3: What happens to PC during an interrupt?
**A:** Current PC (return address) is pushed to stack. New PC loaded from interrupt vector. On return, old PC popped → points to instruction AFTER where interrupt occurred.

### Q4: Can a lower-priority interrupt interrupt a higher-priority handler?
**A:** NO! Only higher-priority can interrupt lower-priority. Lower-priority remains pending.

### Q5: What's stored in PSW?
**A:** Condition codes + interrupt enable/disable bits + processor mode (user/supervisor) + other status info.

### Q6: Is a system with one CPU + GPU a multiprocessor?
**A:** NO! GPU is a special-purpose processor. Multiprocessor requires multiple GENERAL-PURPOSE processors.

### Q7: What's the difference between Asymmetric and Symmetric multiprocessing?
**A:** Asymmetric = boss-worker, specific tasks per processor. Symmetric = all processors equal, any can do any task.

### Q8: Why is main memory called "real memory"?
**A:** To distinguish from virtual memory (which we'll cover later). It's the actual physical RAM.

### Q9: What does "graceful degradation" mean?
**A:** System continues operating (at reduced performance) when some components fail. Key benefit of multiprocessors.

### Q10: Why are timer interrupts important?
**A:** They allow OS to regain control periodically for scheduling. Without them, a user program could monopolize the CPU forever!

---

## 📚 Key Terms Glossary

| Term | Definition |
|------|------------|
| **PC** | Program Counter - address of next instruction |
| **IR** | Instruction Register - current instruction |
| **PSW** | Program Status Word - processor state info |
| **ISR** | Interrupt Service Routine - handler program |
| **DMA** | Direct Memory Access - bypasses CPU for I/O |
| **SMP** | Symmetric Multiprocessing - all processors equal |
| **SAN** | Storage Area Network - shared storage for clusters |
| **Cache Hit** | Data found in cache |
| **Cache Miss** | Data not in cache, must fetch from lower level |
| **Volatile** | Loses data when power off |
| **Graceful Degradation** | System continues working with failed components |
| **Hot-Standby** | Backup system ready to take over immediately |

---

## 🔗 Quick Reference Links (for Obsidian)

- [[#What is an Operating System]]
- [[#Basic Elements of a Computer System]]
- [[#CPU Internal Structure]]
- [[#Instruction Execution Cycle]]
- [[#INTERRUPTS]]
- [[#Multiple Interrupts]]
- [[#Storage-Device Hierarchy]]
- [[#Caching Principle]]
- [[#I/O Techniques Comparison]]
- [[#Multiprocessor Systems]]
- [[#SMP Architecture]]
- [[#Clustered Systems]]
- [[#EXAM TRICKY QUESTIONS]]

---

> **Study Tip:** Focus on the diagrams and flowcharts - exam questions often ask you to trace through interrupt handling or compare I/O techniques. Know the register names and their functions by heart!

> **Last Updated:** Lecture 1 Summary - Introduction & Computer System Overview
