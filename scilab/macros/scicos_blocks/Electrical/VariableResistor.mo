class VariableResistor
	Pin p, n;
	Real R "Resistance";
equation
	R*p.i = p.v - n.v;
	p.i = -n.i;
end VariableResistor;
