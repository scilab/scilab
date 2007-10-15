model CCS "controlled voltage source" 
  Pin p, n;
  Real Iin;
equation 
 Iin = n.i;
 0 = p.i + n.i;
end CCS;
