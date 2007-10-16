model Gyrator "Gyrator" 
  parameter Real G1=1 "Gyration conductance";
  parameter Real G2=1 "Gyration conductance";

  Pin p1,n1,p2,n2;    
  Real v2,v1,i1,i2;

equation 

  v1=p1.v-n1.v;
  0=p1.i+n1.i;
  i1=p1.i;

  v2=p2.v-n2.v;
  0=p2.i+n2.i;
  i2=p2.i;

  i1 = G2*v2;
  i2 = -G1*v1;

end Gyrator;
