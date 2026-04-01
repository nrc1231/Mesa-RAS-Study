### Modelica Physical Dynamics Module

This folder contains the physical dynamic model used in this Mesa system case study, implemented in Modelica.

Main components:
- Full-car active suspension dynamics: active.mo  
  Implements the full-vehicle active suspension system, including body and wheel dynamics.

- Road disturbance generator: RoadStep4W.mo  
  Provides four-wheel road excitation inputs based on predefined or stochastic ISO standard road profiles.

- Full-car passive suspension baseline: passive.mo  
  Serves as a reference model for performance comparison with active control strategies.

- TCP communication interface: socket_client.c  
  Enables real-time data exchange with the Lingua Franca (LF) server.

- Auto load script: package.mo  
  Organizes and initializes the Modelica package.

<img width="1494" height="876" alt="af6a3e79-0f73-47f7-a3fb-ec81bd03729a" src="https://github.com/user-attachments/assets/10e05af6-8591-4319-8707-55ee886e4469" /> 

A successful loaded program should be like shown above.

<img width="1493" height="876" alt="eb105864-3af5-43e4-9c60-58b76573d3bb" src="https://github.com/user-attachments/assets/2ddd2441-17cb-46f0-ab5b-27a8d8885880" />   

A properly configured simulation setup is shown above.

---

Co-simulation interface:
The Modelica model exchanges simulation data with the Lingua Franca server through a TCP-based communication interface.

At each simulation step:
- Vehicle dynamic states (e.g., body acceleration, suspension deflection, wheel velocities) are sent to LF
- Control inputs (e.g., suspension forces) are received from LF

This enables tightly coupled co-simulation across heterogeneous simulation domains.


Role of Modelica in the Black Mesa System:
The Modelica subsystem serves as the physical plant model (ground-truth dynamics layer) in the Black Mesa System.

It is responsible for:
- Providing high-fidelity multi-body vehicle dynamics
- Acting as the continuous-time simulation backbone
- Supplying realistic system responses to control algorithms implemented in LF, SystemC-AMS, and GVSoC
- Serving as the reference environment for evaluating control strategies (e.g., PID, skyhook, regenerative suspension control)

---

System integration perspective:
Within the overall architecture:
- Modelica handles physics and dynamics
- Lingua Franca handles orchestration and scheduling
- SystemC-AMS models physical/electrical subsystems
- GVSoC executes embedded control software

The Modelica subsystem closes the control loop with LF and enables consistent cross-domain co-simulation.
