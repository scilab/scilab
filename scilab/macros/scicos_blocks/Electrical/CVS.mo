model CVS "controlled voltage source" 
  Pin p, n;
  Real vin;
equation 
 vin = p.v - n.v;
 0 = p.i + n.i;
end CVS;
