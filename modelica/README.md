This folder contains the physical dynamic model used in the Black Mesa System.

Main components:
- Full car active suspension dynamics: active.mo.
- Road disturbance generator: RoadStep4W.mo.
- Full car passive suspension baseline: passive.mo.
- TCP communication interface for LF coupling: socket_client.c.
- Auto load script: package.mo.

The Modelica model exchanges simulation data with the Lingua Franca server through the TCP interface.
The Modelica subsystem serves as the physical plant model in the Black Mesa System.
It provides vehicle dynamic states to the Lingua Franca server during co-simulation.
