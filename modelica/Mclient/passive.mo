within Mclient;


model passive
  
  output Real a_s ;
  output Real acc_body "Body vertical acceleration (m/s2)";
  output Real v_body_out "Body vertical velocity (m/s)";

  parameter Real ms = 300*4 ;
  parameter Real mu = 60    ;
  parameter Real ks = 16000 ;
  parameter Real cs = 1000  ;
  parameter Real kt = 190000 ;

  
  parameter Real a = 1.2 ;
  parameter Real b = 1.6 ;
  parameter Real t = 1.55 ;

  
  parameter Real Iy = 2500 ;
  parameter Real Ix = 2200 ;

  
  Real z_body(start=0)  ;
  Real v_body(start=0)  ;

  Real pitch(start=0)      ;
  Real pitch_rate(start=0) ;

  Real roll(start=0)       ;
  Real roll_rate(start=0)  ;

  
  Real zu_FL(start=0) ;
  Real zu_FR(start=0) ;
  Real zu_RL(start=0) ;
  Real zu_RR(start=0) ;

  Real vu_FL(start=0) ;
  Real vu_FR(start=0) ;
  Real vu_RL(start=0) ;
  Real vu_RR(start=0) ;

  
  input Real zr_FL;
  input Real zr_FR;
  input Real zr_RL;
  input Real zr_RR;

  
  Real z_FL, z_FR, z_RL, z_RR;
  Real v_FL, v_FR, v_RL, v_RR;
  Real acc_FL, acc_FR, acc_RL, acc_RR;

  
  Real Fs_FL, Fs_FR, Fs_RL, Fs_RR;
  Real Ft_FL, Ft_FR, Ft_RL, Ft_RR;

equation
 
  z_FL = z_body + a*pitch + (t/2)*roll;
  z_FR = z_body + a*pitch - (t/2)*roll;
  z_RL = z_body - b*pitch + (t/2)*roll;
  z_RR = z_body - b*pitch - (t/2)*roll;

  v_FL = v_body + a*pitch_rate + (t/2)*roll_rate;
  v_FR = v_body + a*pitch_rate - (t/2)*roll_rate;
  v_RL = v_body - b*pitch_rate + (t/2)*roll_rate;
  v_RR = v_body - b*pitch_rate - (t/2)*roll_rate;

  acc_FL = der(v_FL);
  acc_FR = der(v_FR);
  acc_RL = der(v_RL);
  acc_RR = der(v_RR);

  
  Fs_FL = ks*(z_FL - zu_FL) + cs*(v_FL - vu_FL);
  Fs_FR = ks*(z_FR - zu_FR) + cs*(v_FR - vu_FR);
  Fs_RL = ks*(z_RL - zu_RL) + cs*(v_RL - vu_RL);
  Fs_RR = ks*(z_RR - zu_RR) + cs*(v_RR - vu_RR);

  
  Ft_FL = kt*(zu_FL - zr_FL);
  Ft_FR = kt*(zu_FR - zr_FR);
  Ft_RL = kt*(zu_RL - zr_RL);
  Ft_RR = kt*(zu_RR - zr_RR);

  
  ms*der(v_body) = -(Fs_FL + Fs_FR + Fs_RL + Fs_RR);

  Iy*der(pitch_rate) = -( a*(Fs_FL + Fs_FR) - b*(Fs_RL + Fs_RR) );
  Ix*der(roll_rate)  = -( (t/2)*(Fs_FL - Fs_FR + Fs_RL - Fs_RR) );

  
  mu*der(vu_FL) = Fs_FL - Ft_FL;
  mu*der(vu_FR) = Fs_FR - Ft_FR;
  mu*der(vu_RL) = Fs_RL - Ft_RL;
  mu*der(vu_RR) = Fs_RR - Ft_RR;

  
  der(z_body) = v_body;
  der(pitch)  = pitch_rate;
  der(roll)   = roll_rate;

  der(zu_FL) = vu_FL;
  der(zu_FR) = vu_FR;
  der(zu_RL) = vu_RL;
  der(zu_RR) = vu_RR;

  
  acc_body   = der(v_body);
  v_body_out = v_body;
  a_s        = acc_body;  

end passive;
