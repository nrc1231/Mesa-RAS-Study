# Lingua Franca Component

This directory contains the **Lingua Franca (LF)** programs used in the suspension co-simulation framework.

Lingua Franca is used as the **coordination and communication layer** between different simulation components in Mesa system.

In this project, LF orchestrates the interaction between:

- SystemC-AMS simulation modules
- GVSoC embedded software
- external simulation tools

The LF runtime is responsible for scheduling tasks and managing communication between the simulation subsystems.

---

# Directory Structure

lf/

src/       → Generated C++ source code  

---

# Complie the LF server

These server is generated automatically by the Lingua Franca compiler.
Navigate to the `lf/src` directory and compile the LF program:

```bash
cd lf/src
lfc mesa2.lf
cd ../bin  //The compiled executable will be generated in the lf/bin directory.
mesa2.exe
```

---

# Requirements

To compile and run the LF components, the following tools are required:

- Lingua Franca compiler
- Java 17 or newer
- C++ compiler (gcc or g++)
- Linux environment (recommended)

Example tested environment:

Ubuntu 22.04  
Java 17  
gcc 11  

---

# Running the LF Runtime

After compilation, run the generated executable:

./bin/<program>

This starts the Lingua Franca runtime which coordinates the simulation components.   

<img width="1113" height="626" alt="77c4f089-55d8-4899-b1ca-9677dc5ca83d" src="https://github.com/user-attachments/assets/1b417c03-f79b-47d6-bf87-7b265afcfcc6" />                 
 
A successful execution should result in an idle system state, as illustrated above.

---

# Role of Lingua Franca in This Project

In this suspension simulation framework, Lingua Franca acts as the **central orchestration layer**.

It coordinates communication and scheduling between multiple simulation environments.

The interaction between the main components is structured as follows:

Modelica vehicle simulation
↑   ↓
Lingua Franca orchestration
↑   ↓
SystemC-AMS physical simulation / GVSoC execution

Lingua Franca manages the timing and communication between these components.

---

# Notes

The LF programs provided in this directory are designed specifically for the suspension simulation experiments included in this project.

They may require modification if used in other simulation environments.

---
