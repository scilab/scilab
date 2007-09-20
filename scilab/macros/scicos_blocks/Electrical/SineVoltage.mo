model SineVoltage "Sine voltage source" 
  parameter Real V=1 "Amplitude of sine wave";
  parameter Real phase=0 "Phase of sine wave";
  parameter Real freqHz=1 "Frequency of sine wave";
  parameter Real offset=0 "Offset volatge of sine wave";
  parameter Real startTime=0 "sine wave start time";
  parameter Real PI=3.141592653589793;
  Pin p, n;
  Real v;
  
equation

  v = offset + (if time < startTime then 0 else V*Modelica.Math.sin(2*PI*freqHz*(time - startTime) +phase));
  v= p.v - n.v;	
  0 = p.i + n.i;
end SineVoltage;


