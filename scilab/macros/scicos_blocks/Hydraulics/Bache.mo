class Bache 
  parameter Real Patm "Pression dans le ciel de la bache";
  parameter Real A "Section de la bache";
  parameter Real ze1 "Altitude du piquage d'entrée 1";
  parameter Real ze2 "Altitude du piquage d'entrée 2";
  parameter Real zs1 "Altitude du piquage de sortie 1";
  parameter Real zs2 "Altitude du piquage de sortie 2";
  parameter Real z0 "Altitude initiale du fluide";
  parameter Real T0 "Temperature initiale du fluide";
  parameter Real p_rho 
    "Si > 0, masse volumique imposée du fluide";
//  parameter Integer mode "Région IF97. 1:liquide - 2:vapeur - 4:saturation - 0:calcul automatique";
  
  parameter Real g=9.80665 "Accélération de la pesanteur";
  parameter Real e=0.25 "Ouverture des piquages";
  parameter Real k=1
    "Coefficient de perte de charge des piquages";
  parameter Real eps=1.e-0 "Limite inf. de la fonction carrée";
  parameter Real zmin=1.e-6 
    "Hauteur minimum d'eau pour l'équation de l'énergie";
  Real z(start=z0) "Hauteur du fluide";
  Real Tm(start=T0) "Température moyenne";
  Real Pm(start=1.e5) "Pression moyenne";
  Real Hm(start=100000) "Enthalpie spécifique moyenne";
  Real rho(start=998) "Masse volumique";
  Real BQ "Bilan de masse";
  Real BH "Bilan d'énergie";
  Real Oe1;
  Real Oe2;
  Real Os1;
  Real Os2;
  Real region(start=1) "Numéro de région IF97";
  Real Max_e1;
  Real Max_e2;
  Real Max_s1;
  Real Max_s2;
  Real ThermoCar1;
  Real ThermoCar2;

  Real yNiveau ;

public 
//  SortieReelle yNiveau ;
  PortPHQ1 Ce1 ;
  PortPHQ2 Cs2 ;
//  PortTher Cth ; /* Port non utilisé dans le modèle désiré, non implanté sur le bloc SCICOS */
  PortPHQ1 Ce2 ;
  PortPHQ2 Cs1 ;

//initial equation 

//  Hm = 100000;
  
equation

  Ce2.Q = 0;
  Ce2.H = 1.e5;

  Cs1.Q = 0;
  Cs1.H = 1.e5;

  Max_e1 = if (z - ze1 < 0.0) then 0.0 else (z - ze1);
  ThermoCar1 = if (abs(Ce1.Q) > eps) then Ce1.Q*abs(Ce1.Q) else Ce1.Q*eps;
  (Patm + rho*g*Max_e1 - Ce1.P)*Oe1 = k*ThermoCar1/2/rho;

  Oe1 = if (Ce1.Q >= 0.0) then 1.0
	elseif (z > ze1 + e) then 1.0
	elseif (z < ze1) then 0.0
	else (z - ze1)/e;


   Max_e2 = if (z - ze2 < 0.0) then 0.0 else (z - ze2);

  Ce2.P = Patm + rho*g*Max_e2;
  Oe2 = 0;

  Max_s1 = if (z - zs1 < 0.0) then 0.0 else (z - zs1);
  Cs1.P = Patm + rho*g*Max_s1;
  Os1 = 0;

  Max_s2 = if (z - zs2 < 0.0) then 0.0 else (z - zs2);
  ThermoCar2 =if (abs(Cs2.Q) > eps) then Cs2.Q*abs(Cs2.Q) else Cs2.Q*eps;
  (Patm + rho*g*Max_s2 - Cs2.P)*Os2 = k*ThermoCar2/2/rho;

  Os2 = if (Cs2.Q <= 0.0) then 1.0
    elseif (z > zs2 + e) then 1.0
    elseif (z < zs2) then 0.0
    else (z - zs2)/e;


  /* Bilan de masse */
  BQ = Ce1.Q + Ce2.Q - Cs1.Q - Cs2.Q;
  rho*A*der(z) = BQ;
  
  /* Bilan d'énergie */
  /* le port thermique n'est pas utilisé ici  "+ Cth.W" (on a W=0)*/
  BH = Ce1.Q*(Ce1.H - Hm) + Ce2.Q*(Ce2.H - Hm) - Cs1.Q*(Cs1.H - Hm) - Cs2.Q*(Cs2.H - Hm);

  der(Hm) = if (z > zmin) then BH/(rho*A*z) else 0.0;

  Ce1.Hm = Hm;
  Ce2.Hm = Hm;
  Cs1.Hm = Hm;
  Cs2.Hm = Hm;
  
//  Cth.T = Tm; /* le port thermique n'est pas utilisé ici*/
  
  /* Capteur de niveau */
  yNiveau = z;
  
  /* Calcul des propriétés thermodynamiques */
  Pm = Patm + rho*g*z/2;
  
  Tm = Hm/4187 + 273.15;
  region = 1.0;
  
  rho = 998.0;

end Bache;
