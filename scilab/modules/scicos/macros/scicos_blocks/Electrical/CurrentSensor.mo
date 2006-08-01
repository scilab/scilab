 class CurrentSensor 
  Pin p;
  Pin n;
  Real i;
 equation
    p.v = n.v;
    p.i = i;
    n.i = -i;
 end CurrentSensor; 

