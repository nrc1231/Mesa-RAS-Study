```mermaid
flowchart TB
    Modelica["Modelica<br/>Vehicle Dynamics"]
    LF["Lingua Franca<br/>Orchestrator"]
    SCAMS["MESSY<br/> SystemC-AMS + GVSoC"]

    Modelica <-->|TCP / Socket| LF
    LF <-->|TCP / Socket| SCAMS
```


# Black Mesa System

The **Black Mesa System** is a co-simulation framework designed for cyber-physical system research, with a focus on multi-domain integration and deterministic orchestration.

It enables coordinated execution across heterogeneous simulation environments, including:

- physical system modeling (e.g., vehicle dynamics in Modelica)
- system-level orchestration (Lingua Franca)
- hardware-oriented simulation (SystemC-AMS)
- embedded software execution (GVSoC / RISC-V)

---

## Current Research Focus

This project has been applied to the study of **regenerative suspension systems**, where control strategies, energy recovery mechanisms, and physical dynamics are jointly simulated.

---

## General Capability

Beyond regenerative suspension, the framework is designed as a **general-purpose co-simulation platform**, capable of supporting:

- advanced vehicle control systems (active suspension, ADAS)
- sensor fusion and perception-driven control
- hardware-in-the-loop (HIL) experiments
- cyber-physical system validation
- cross-layer simulation from physics to embedded software

The architecture allows flexible extension to other domains where tight coupling between physical models, control logic, and embedded execution is required.

# System Architecture

The Black Mesa System is composed of multiple layers that interact through a co-simulation pipeline.

The execution flow of the system is:

Lingua Franca orchestration  
↓  
MESSY SystemC-AMS simulation (GVSoC environment)  
↓  
Modelica physical vehicle model  

Lingua Franca acts as the coordination layer that manages communication between the different simulation environments.

---

# Repository Structure
BlackMesaSystem
│
├── modelica
│ Modelica physical dynamic models
│ Vehicle suspension dynamics and road disturbances
│
├── lf
│ Lingua Franca orchestration programs
│ Coordinates communication between simulation modules
│
├── docker
│ Docker environment containing the MESSY framework
│ Includes SystemC-AMS and GVSoC simulation environment
│
└── README.md

Each directory contains additional documentation explaining the corresponding subsystem.

---

# Components

## Modelica

The **Modelica** subsystem implements the physical dynamic model of the vehicle suspension system.

Main components include:

- full car suspension dynamics
- road disturbance generator
- passive suspension baseline
- TCP communication interface for LF coupling

See:

modelica/README.md

---

## Lingua Franca

The **Lingua Franca (LF)** subsystem provides the orchestration layer of the co-simulation framework.

It is responsible for:

- scheduling simulation execution
- coordinating data exchange
- managing communication between subsystems

See:

lf/README.md

---

## MESSY / SystemC-AMS Environment

The **docker** directory contains the simulation environment based on the **MESSY framework**.

This environment includes:

- SystemC-AMS simulation modules
- GVSoC embedded system simulation
- sensor and power system models
- communication interfaces for co-simulation

The environment is distributed as a Docker image to ensure reproducibility.

See:

docker/README.md

---

# Running the Simulation

The simulation requires three components to be started in the correct order.

Execution order:

1. Lingua Franca server  
2. MESSY SystemC-AMS simulation  
3. Modelica physical simulation  

---

# Step 1 — Start Lingua Franca Server

Navigate to the LF directory:

cd lf

Compile the LF program if necessary:

lfc server.lf

Run the server:

./bin/server

The LF server must remain running during the entire simulation.

---

# Step 2 — Start the MESSY Simulation

Open a new terminal.

Start the Docker container containing the MESSY framework:

docker run -it messy_image

Inside the container compile the application:

make application app=/messy/examples/read_write_sensor

Then run the simulation:

make run

---

# Step 3 — Start the Modelica Simulation

Open another terminal.

Navigate to the Modelica directory:

cd modelica

Launch the Modelica simulation environment and run the vehicle suspension model.

The Modelica model exchanges data with the Lingua Franca server through the TCP communication interface.

---

# Execution Summary

The complete workflow is:

Start Lingua Franca server  
↓  
Start MESSY simulation  
↓  
Start Modelica simulation  

The system components will then exchange data during the co-simulation process.

---

# Requirements

Recommended environment:

Linux (Ubuntu 22.04)  
Docker  
Lingua Franca compiler  
OpenModelica  
C++ compiler (gcc)

---

# Research Purpose

The Black Mesa System was developed to support research on:

- vehicle suspension dynamics
- cyber-physical systems
- co-simulation frameworks
- embedded control system integration

The framework enables coordinated experimentation across physical models and embedded system simulations.

---

# Notes

This repository provides the integration framework and modified components required to reproduce the suspension co-simulation environment.

Some components are derived from external open-source projects.  
Please refer to the original upstream repositories for their complete source code and licensing terms.

---

# Contact

For questions regarding the Black Mesa System or reproduction instructions, please contact the repository authors.

This repository is released under the MIT License.
See the LICENSE file for details.
