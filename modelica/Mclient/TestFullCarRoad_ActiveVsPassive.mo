within Mclient;

model TestFullCarRoad_ActiveVsPassive  
  
passive car_passive;

 
  FullCar4W_PassiveFromActiveSuspension car_active;

  
  RoadStep4W road;
  TCPClient cli;
  Real dummy(start=0);
equation
  
  car_passive.zr_FL = road.zr_FL;
  car_passive.zr_FR = road.zr_FR;
  car_passive.zr_RL = road.zr_RL;
  car_passive.zr_RR = road.zr_RR;

  
  car_active.zr_FL = road.zr_FL;
  car_active.zr_FR = road.zr_FR;
  car_active.zr_RL = road.zr_RL;
  car_active.zr_RR = road.zr_RR;
  
  cli.afl= car_active.acc_FL;
  cli.afr= car_active.acc_FR;
  cli.arl= car_active.acc_RL;
  cli.arr= car_active.acc_RR;
  
  //cli.afl= car_active.v_FL;
  //cli.afr= car_active.v_FR;
  //cli.arl= car_active.v_RL;
  //cli.arr= car_active.v_RR;
  
  cli.vrel1= car_active.vrel_FL;
  cli.vrel2= car_active.vrel_FR;
  cli.vrel3= car_active.vrel_RL;
  cli.vrel4= car_active.vrel_RR;
  
  car_active.u_FL=cli.controlForce;
  car_active.u_FR=cli.controlForce2;
  car_active.u_RL=cli.controlForce3;
  car_active.u_RR=cli.controlForce4;
 
  der(dummy) = 1;

end TestFullCarRoad_ActiveVsPassive;



