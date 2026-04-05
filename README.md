# Distributed Stoplight System using GPIO (PMOD Interface)

## Overview
This project implements a distributed stoplight control system using two embedded boards communicating through **PMOD GPIO interfaces**. The system simulates real-world traffic light coordination by passing signals between two processors, where one board controls **North-South (N/S)** traffic and the other controls **East-West (E/W)** traffic.

The focus of this project is on **low-level GPIO communication, synchronization between processors, and real-time state control** without relying on high-level communication protocols.

---

## Features
- Inter-board communication using GPIO (PMOD interface)
- Real-time synchronization between two embedded systems
- Traffic light state machine implementation
- Deterministic timing control using delays
- Hardware-level register manipulation
- Safe communication protocol design between processors

---

## System Architecture

### Hardware Components
- Two ARM-based embedded boards (Zynq / Blackboard)
- PMOD GPIO interface for communication
- On-board LEDs representing traffic lights

### Communication Design
- One board acts as **N/S controller**
- One board acts as **E/W controller**
- GPIO pins are used to send control signals between boards
- Output pins from one board connect to input pins of the other

---

## GPIO Register Mapping

The system uses direct memory-mapped I/O:

```c
#define Bank0_Input   *((uint32_t *)0xE000A060)
#define Bank2_Input   *((uint32_t *)0xE000A068)

#define Bank0_Output  *((uint32_t *)0xE000A040)
#define Bank2_Output  *((uint32_t *)0xE000A048)

#define Bank0_Dir     *((uint32_t *)0xE000A204)
#define Bank2_Dir     *((uint32_t *)0xE000A284)

#define Bank0_Enable  *((uint32_t *)0xE000A208)
#define Bank2_Enable  *((uint32_t *)0xE000A288)

## GPIO Configuration
- Configures GPIO direction (input/output)  
- Enables GPIO banks  
- Reads and writes data directly to hardware registers  

---

## Communication Protocol

A simple signaling protocol is implemented between boards:

- `1` → Activate signal (e.g., green light)  
- `0` → Deactivate signal (red light)  

Each board:
- Sends its current state via GPIO outputs  
- Reads the other board’s state via GPIO inputs  
- Updates LEDs accordingly  

---

## Traffic Light State Machine

The system follows a synchronized sequence:

1. Both directions start in red  
2. N/S turns green, E/W remains red  
3. N/S transitions to yellow  
4. N/S turns red  
5. E/W turns green  
6. E/W transitions to yellow  
7. E/W turns red  
8. Repeat cycle  

---

## Timing Control
- Green: 2 seconds  
- Yellow: 2 seconds  
- Red transition: 1 second  

---

## How It Works

1. Initialize GPIO banks  
2. Configure PMOD pins:  
   - Outputs for transmitting signals  
   - Inputs for receiving signals  
3. Assign board roles (N/S or E/W)  

### Continuous Operation
- Send current light state  
- Read incoming signals  
- Update LEDs  
- Follow timing sequence  

---

## Key Concepts Demonstrated
- Inter-processor communication without shared memory  
- GPIO-based signaling systems  
- Embedded synchronization techniques  
- State machine design in embedded systems  
- Memory-mapped register programming  

---

## Performance Considerations
- No communication overhead from protocols like UART/I2C  
- Fast signal propagation through GPIO  
- Deterministic execution timing  
- Minimal resource usage  

---

## Potential Improvements
- Replace delay-based timing with hardware timers  
- Add pedestrian crossing signals  
- Implement interrupt-driven communication  
- Expand to multi-intersection coordination  
- Add fault detection for communication failure  

