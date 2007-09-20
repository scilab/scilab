model Resistor
	Pin p, n;
	parameter Real R "Resistance";

equation
	R*p.i = p.v - n.v;
	p.i = -n.i;

end Resistor;
