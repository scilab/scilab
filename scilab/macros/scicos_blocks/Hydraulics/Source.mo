model Source
  parameter Real P0=300000 "Pression de la source";
  parameter Real T0=290 "Température de la source";
  parameter Real H0=100000 "Enthalpie spécifique de la source";
  parameter Real option_temperature=1 "1:température fixée - 2:enthalpie fixée";
   
  Real Pm "Pression moyenne";
  Real Q "Débit";
  Real Tm "Température moyenne";
  Real Hm "Enthalpie spécifique moyenne";

public 
  PortPHQ2 C ;
equation 
  
  C.P = Pm;
  C.Q = Q;
  C.Hm = Hm;

  Pm = P0;

  Hm = H0;
  
  Tm = if (option_temperature <= 1) then T0 else Hm/4187 + 273.15;
  
end Source;
