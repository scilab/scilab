model Puits
  parameter Real P0=100000 "Pression du puits";
  parameter Real T0=290"Température du puits";
  parameter Real H0=100000 "Enthalpie spécifique du puits";
  parameter Real option_temperature=1 "1:température fixée - 2:enthalpie fixée";

  Real Pm "Pression moyenne";
  Real Q "Débit";
  Real Tm "Température moyenne";
  Real Hm "Enthalpie spécifique moyenne";

public 
  PortPHQ1 C ;
equation 
  
  C.P = Pm;
  C.Q = Q;
  C.Hm = Hm;

  Pm = P0;
  Hm= H0;

  
  Tm = if (option_temperature <= 1) then T0 else Hm/4187 + 273.15;

end Puits;
