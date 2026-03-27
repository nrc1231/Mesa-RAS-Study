# MESSY Suspension Simulation – Reproduction Guide

This repository provides the modified source files required to reproduce the suspension simulation environment based on the **MESSY framework**: https://github.com/eml-eda/messy.git.

The original framework must be obtained separately.  
This repository only contains the modified files and instructions needed to reproduce our environment.

---

# 1. Obtain the MESSY Docker Image

Download the **MESSY docker** file by following the steps seen in the following link：https://eml-eda.github.io/messy/getting-started/ 

```text
git clone https://github.com/eml-eda/messy
cd messy
```

Load the docker image:

```text
docker build . -f docker/pulp-open/Dockerfile -t messy --build-arg USER_ID=$(id -u ${USER}) --build-arg GROUP_ID=$(id -g ${USER})
```

Run the container:
```text
docker run -it -p 3000:3000 --user root messy /bin/bash
```
---

# 2. Run the Original Example Once

Inside the container, compile and run the default example to generate the initial GVSoC build environment.

```text
python3 messy.py -f codegen/pulp_open.json -a /messy/examples/read_write_sensor
```
```text
make application app=/messy/examples/read_write_sensor
```

This step generates the necessary simulation code and build environment.

After the example is successfully built, terminate the example execution.

---

# 3. Replace the Example Sensor Source File

Navigate to the example directory:

/messy/examples/read_write_sensor

Replace the file:

read_write_sensor.c

with the modified version provided in this repository: /docker/MESSY/example/.

---

# 4. Replace the SystemC-AMS Source Files

Navigate to the source directory:

/messy/messy/src/

Replace the following files with the modified versions provided in this repository: /docker/MESSY/src/

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

Navigate to the source directory:
/messy/messy/include/

Replace the following files with the modified versions provided in this repository: /docker/MESSY/include/
core.hpp  
core_power.hpp  
functional_bus.hpp  
harvester_battery.hpp  
lut.hpp  
messy_request.hpp  
power_bus.hpp  
sensor_mic_click_functional.hpp  
sensor_mic_click_power.hpp  

Navigate to the source directory:

/messy/messy/src/converter/

Replace the following files with the modified versions provided in this repository: /docker/MESSY/src/converter/
battery_converter.cpp
core_converter.cpp

Navigate to the source directory:

/messy/messy/include/converter/

Replace the following files with the modified versions provided in this repository: /docker/MESSY/src/converter/
battery_converter.hpp
core_converter.hpp


These files implement the modified SystemC-AMS behaviour required for the regenerative suspension co-simulation framework.

---

# 5. Recompile the Application

Rebuild the application so that the modified SystemC-AMS modules and sensor interface are compiled into the GVSoC simulation.

Run: 
```text
make application app=/messy/examples/read_write_sensor
```
---

# 6. Run the Simulation

Finally run the simulation:
```text
make run
```
---

# Workflow Summary

docker load messy image  
↓  
run container  
↓  
make application: read_write_sensor (generate baseline build)  
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

Example application in GvSoC:

read_write_sensor.c

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
battery_converter.cpp
core_converter.cpp


core.hpp  
core_power.hpp  
functional_bus.hpp  
harvester_battery.hpp  
lut.hpp  
messy_request.hpp  
power_bus.hpp  
sensor_mic_click_functional.hpp  
sensor_mic_click_power.hpp  
battery_converter.hpp
core_converter.hpp

---

# Notes

The modifications adapt the MESSY framework to support the suspension co-simulation architecture used in our research.

The modified modules integrate:

- SystemC-AMS models
- sensor interface logic
- controller behavior simulation
- energy harvesting behaviour

These components are used as part of a larger simulation workflow including Modelica vehicle models and Lingua Franca orchestration.
