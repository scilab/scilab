model VoltageSensor 
  Pin p;
  Pin n;
  Real v;
 equation
    p.i = 0;
    n.i = 0;
    v = p.v - n.v;
 end VoltageSensor;

