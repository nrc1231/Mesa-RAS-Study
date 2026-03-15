within Mclient;

model RoadStep4W
  output Real zr_FL "FL";
  output Real zr_FR "FR";
  output Real zr_RL "RL";
  output Real zr_RR "RR";

  parameter Real wheelbase = 2.8 "m";
  parameter Real v = 20 "m/s";
  parameter Real Ts = 0.001;

  parameter Real f0 = 0.1;
  parameter Real n0 = 0.1;
  parameter Real Gq = 356e-3;

  
  parameter Real tDelay = wheelbase / v "Rear axle delay time (s)";

  Real tpos   "front axle position (m)";
  Real zr_base(start=0);
  Real zr_dot;

  discrete Integer seed(start=1, fixed=true);
  discrete Real w(start=0, fixed=true);

equation
  
  tpos = v*time;

 
  when sample(0,Ts) then
    seed = integer(mod(1103515245*pre(seed) + 12345, 2147483647));
    w = 2.0*(seed/2147483647.0 - 0.5);
  end when;

 
  zr_dot = -2*Modelica.Constants.pi*f0*zr_base
           + 2*Modelica.Constants.pi*n0*w*sqrt(Gq*v);

  der(zr_base) = zr_dot;

 
  zr_FL = zr_base;
  zr_FR = zr_base;

  
  zr_RL = if v > 1e-3 then delay(zr_base, tDelay) else zr_base;
  zr_RR = if v > 1e-3 then delay(zr_base, tDelay) else zr_base;

end RoadStep4W;

