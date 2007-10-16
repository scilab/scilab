model IdealTransformer "Transformer with two ports" 
  parameter Real N=1 "Transformer turn ration N1/N2";  
  Pin p1, n1, p2, n2;    
  Real v2,v1,i1,i2;

equation 
  v1=p1.v-n1.v;
  0=p1.i+n1.i;
  i1=p1.i;

  v2=p2.v-n2.v;
  0=p2.i+n2.i;
  i2=p2.i;

/*  v1 = L1*der(i1) + M*der(i2); 
  v2 = M*der(i1) + L2*der(i2);
    Results very often in high index
  problem not handled in Scicos /Masoud
*/

v1=N*v2;
i2=-N*i1;

end IdealTransformer;

