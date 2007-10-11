class OpAmp 
  parameter Real OLGain=1000 "Open Loop gain";
  parameter Real SatH=10  "Positive saturation voltage";
  parameter Real SatL=-10 "Negative Saturation voltage";
  Pin in_p "Positive pin of the input port";
  Pin in_n "Negative pin of the input port";
  Pin out "Output pin";
  Real outx;
equation 
  in_p.i = 0;
  in_n.i = 0;
  outx= OLGain*(in_p.v - in_n.v);
  out.v=if outx> SatH then SatH
                      else if outx<SatL then SatL
                                        else outx;   
end OpAmp;



