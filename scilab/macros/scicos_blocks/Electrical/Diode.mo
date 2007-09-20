model Diode "Simple diode" 
   Pin p, n;
    parameter Real Ids=1.e-6 "Saturation current";
    parameter Real Vt=0.04   "Voltage equivalent of temperature (kT/qn)";
    parameter Real Maxexp=15 "Max. exponent for linear continuation";
    parameter Real R=1.e8 "Parallel ohmic resistance";
    Real v;
  equation 
   v = p.v - n.v;

   p.i = if noEvent(v/Vt > Maxexp) then 
	Ids*(Modelica.Math.exp(Maxexp)*(1 + v/Vt - Maxexp) - 1) + v/R 
  else 
	Ids*(Modelica.Math.exp(v/Vt) - 1) + v/R;

   p.i = -n.i;

  end Diode;
