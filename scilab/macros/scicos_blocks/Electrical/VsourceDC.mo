class VsourceDC "Constant voltage source"
	Pin p, n;
	Real v;
	parameter Real A = 220 "Amplitude";
equation
	v = A;
end VsourceDC;