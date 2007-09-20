model VVsourceAC "Sin-wave voltage source"
	Pin p, n;
	Real v;
	Real VA "voltage";
	parameter Real f = 50 "Frequency";
	parameter Real PI=3.141592653589793;
equation
	v = VA*Modelica.Math.sin(2*PI*f*time);
	v = p.v - n.v;
	0 = p.i + n.i;
end VVsourceAC;
