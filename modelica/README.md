This folder contains the physical dynamic model used in the Black Mesa System.

Main components:
- Full car suspension dynamics
- Road disturbance generator
- Passive suspension baseline
- TCP communication interface for LF coupling

The Modelica model exchanges simulation data with the Lingua Franca server through the TCP interface.
The Modelica subsystem serves as the physical plant model in the Black Mesa System.
It provides vehicle dynamic states to the Lingua Franca server during co-simulation.