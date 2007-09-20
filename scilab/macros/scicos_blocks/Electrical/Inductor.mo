model Inductor "Ideal electrical inductor"
	Pin p, n;
	Real v;
	Real i;
	parameter Real L "Inductance";
equation
	v = L*der(i);
	v = p.v - n.v;
	0 = p.i + n.i;
	i = p.i;
end Inductor;
