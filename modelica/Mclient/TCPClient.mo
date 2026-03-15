within Mclient;

model TCPClient
  parameter String ip   = "127.0.0.1";
  parameter Integer port = 9000;

  Real afl "acc front lfet";
  Real afr "acc front right";
  Real arl "acc rear lfet";
  Real arr "acc rear right";
  Real cs1;
  Real cs2;
  Real cs3;
  Real cs4;
  Real vrel1;
  Real vrel2;
  Real vrel3;
  Real vrel4;
  
  Real controlForce;
  Real controlForce2;
  Real controlForce3;
  Real controlForce4;
  Real CV (start=0.899995);  //357.372
  

  function init
    input String ip;
    input Integer port;
    external "C" initClient(ip, port)
      annotation(Include="#include \"socket_client.c\"",
                 IncludeDirectory="modelica://Mclient/Resources/Source",
                 Library="ws2_32");
  end init;

  function sendStringToServer
    input String msg;
    external "C" sendString(msg)
      annotation(Include="#include \"socket_client.c\"",
                 IncludeDirectory="modelica://Mclient/Resources/Source",
                 Library="ws2_32");
  end sendStringToServer;

  function recvData
    output String y;
    external "C" y = readFromServerString()
      annotation(Include="#include \"socket_client.c\"",
                 IncludeDirectory="modelica://Mclient/Resources/Source",
                 Library="ws2_32");
  end recvData;

  
  function stringTo4Real
  input String s;
  output Real y1;
  output Real y2;
  output Real y3;
  output Real y4;
  output Real y5;
protected 
  Integer idx;
algorithm
  idx := 1;

  (y1, idx) := Modelica.Utilities.Strings.scanReal(s, idx, false, "");
  idx := Modelica.Utilities.Strings.find(s, ",", idx) + 1;

  (y2, idx) := Modelica.Utilities.Strings.scanReal(s, idx, false, "");
  idx := Modelica.Utilities.Strings.find(s, ",", idx) + 1;

  (y3, idx) := Modelica.Utilities.Strings.scanReal(s, idx, false, "");
  idx := Modelica.Utilities.Strings.find(s, ",", idx) + 1;

  (y4, idx) := Modelica.Utilities.Strings.scanReal(s, idx, false, "");
  idx := Modelica.Utilities.Strings.find(s, ",", idx) + 1;
  
  (y5, idx) := Modelica.Utilities.Strings.scanReal(s, idx, false, "");
end stringTo4Real;


initial algorithm
  init(ip, port);

equation
  when sample(0, 0.001) then
    sendStringToServer(String(afl) + "," + String(afr)+ "," + String(arl)+","+String(arr) +","+String(vrel1)+","+String(vrel2)+","+String(vrel3)+","+String(vrel4)+"\n");
  end when;

  when sample(0.000001, 0.001) then
  (cs1, cs2, cs3, cs4,CV) = stringTo4Real(recvData());
  controlForce = cs1;   
  controlForce2 = cs2;   
  controlForce3 = cs3;   
  controlForce4 = cs4;   
end when;


end TCPClient;
