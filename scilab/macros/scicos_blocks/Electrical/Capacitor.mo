class Capacitor
	Pin p, n;
	Real v;
	parameter Real C "Capacitance";
equation
	C*der(v) = p.i;
	v = p.v - n.v;
	p.i = -n.i;
end Capacitor;
