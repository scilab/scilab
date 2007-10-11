class OpAmp 
//  parameter Real OLGain=1000 "Open Loop gain";
//  parameter Real SatH=10  "Positive saturation voltage";
//  parameter Real SatL=-10 "Negative Saturation voltage";
  Pin in_p "Positive pin of the input port";
  Pin in_n "Negative pin of the input port";
  Pin out "Output pin";
equation 
  in_p.i = 0;
  in_n.i = 0;
  in_p.v - in_n.v=0;
end OpAmp;
  


