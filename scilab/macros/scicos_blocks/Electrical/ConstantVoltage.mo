model ConstantVoltage "Source for constant voltage"
   Pin p, n;
   parameter Real V = 1 "Volts";
equation
   V = p.v - n.v;
   p.i + n.i = 0;
end ConstantVoltage;
