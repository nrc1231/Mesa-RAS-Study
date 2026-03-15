# MESSY Suspension Simulation – Reproduction Guide

This repository provides the modified source files required to reproduce the suspension simulation environment based on the **MESSY framework**.

The original framework must be obtained separately.  
This repository only contains the modified files and instructions needed to reproduce our environment.

---

# 1. Obtain the MESSY Docker Image

Download the prebuilt **MESSY docker image**.

Load the docker image:

docker load -i messy_image.tar

Run the container:

docker run -it messy_image

---

# 2. Run the Original Example Once

Inside the container, compile and run the default example to generate the initial GVSoC build environment.

make application app=/messy/examples/read_write_sensor

This step generates the necessary simulation code and build environment.

After the example is successfully built, terminate the example execution.

---

# 3. Replace the Example Sensor Source File

Navigate to the example directory:

/messy/messy/example/read_write_sensor

Replace the file:

read_write_sensor.c

with the modified version provided in this repository.

Example command:

cp read_write_sensor.c /messy/messy/example/read_write_sensor/

---

# 4. Replace the SystemC-AMS Source Files

Navigate to the source directory:

/messy/messy/src

Replace the following files with the modified versions provided in this repository:

core.cpp  
core_power.cpp  
functional_bus.cpp  
harvester_battery.cpp  
lut.cpp  
main.cpp  
messy_request.cpp  
power_bus.cpp  
sensor_mic_click_functional.cpp  
sensor_mic_click_power.cpp  

Example command:

cp src/* /messy/messy/src/

These files implement the modified SystemC-AMS behaviour required for the suspension co-simulation framework.

---

# 5. Recompile the Application

Rebuild the application so that the modified SystemC-AMS modules and sensor interface are compiled into the GVSoC simulation.

Run:

make application app=/messy/examples/read_write_sensor

---

# 6. Run the Simulation

Finally run the simulation:

make run

---

# Workflow Summary

docker load messy image  
↓  
run container  
↓  
make application (generate baseline build)  
↓  
replace example source file  
↓  
replace SystemC-AMS source files  
↓  
recompile application  
↓  
make run  

---

# Modified Files

Example application:

examples/read_write_sensor/read_write_sensor.c

SystemC-AMS source files:

core.cpp  
core_power.cpp  
functional_bus.cpp  
harvester_battery.cpp  
lut.cpp  
main.cpp  
messy_request.cpp  
power_bus.cpp  
sensor_mic_click_functional.cpp  
sensor_mic_click_power.cpp  

---

# Notes

The modifications adapt the MESSY framework to support the suspension co-simulation architecture used in our research.

The modified modules integrate:

- SystemC-AMS models
- sensor interface logic
- power bus simulation
- energy harvesting behaviour

These components are used as part of a larger simulation workflow including Modelica vehicle models and Lingua Franca orchestration.