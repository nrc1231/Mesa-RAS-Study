# Lingua Franca Component

This directory contains the **Lingua Franca (LF)** programs used in the suspension co-simulation framework.

Lingua Franca is used as the **coordination and communication layer** between different simulation components.

In this project, LF orchestrates the interaction between:

- SystemC-AMS simulation modules
- GVSoC embedded software
- external simulation tools

The LF runtime is responsible for scheduling tasks and managing communication between the simulation subsystems.

---

# Directory Structure

lf/

src/       → Generated C++ source code  

These files are generated automatically by the Lingua Franca compiler.
---
# Complie the LF server

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

# Building the LF Program

Navigate to the LF directory:

cd lf

Compile the LF program using the Lingua Franca compiler:

lfc <program>.lf

This command generates the C++ runtime and source files in the following directories:

include/  
src/  
lib/  

---

# Running the LF Runtime

After compilation, run the generated executable:

./bin/<program>

This starts the Lingua Franca runtime which coordinates the simulation components.

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

# Reference

Lingua Franca Project

https://www.lf-lang.org
