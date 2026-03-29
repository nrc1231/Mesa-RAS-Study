### Modelica Physical Dynamics Module

This folder contains the physical dynamic model used in the Black Mesa System, implemented in Modelica.

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
