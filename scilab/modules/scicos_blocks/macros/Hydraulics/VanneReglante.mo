/*  Scicos
 *
 *  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * See the file ../license.txt
*/

model VanneReglante 
  parameter Real Cvmax "CV max";
  parameter Real p_rho "Si > 0, masse volumique imposée du fluide";
//  parameter Integer mode=0 "Région IF97. 1:liquide - 2:vapeur - 4:saturation - 0:calcul automatique";

  parameter Real eps=1.e-0 "Limite inf. de la fonction carrée";
  Real Cv "Cv";
  Real Q(start=500) "Débit massique";
  Real deltaP "Perte de charge singulière";
  Real rho(start=998) "Masse volumique";
  Real Tm(start=290) "Température moyenne";
  Real Pm(start=1.e5) "Pression moyenne";
  Real Hm(start=100000) "Enthalpie spécifique moyenne";
  Real region(start=1) "Numéro de région IF97";
  Real ThermoCar;

  Real Ouv ;

public 
//  EntreeReelle Ouv ;
  PortPHQ1 C1 ;
  PortPHQ2 C2 ;

equation 
  
  C1.H = C2.H;
  C1.Q = C2.Q;
  
  Q = C1.Q;
  deltaP = C1.P - C2.P;
  
  /* Inversions de débit */
  0 = if (Q > 0.0) then C1.H - C1.Hm else C2.H - C2.Hm;
  
  /* Calcul de la perte de charge */

  ThermoCar = if (abs(Q) > eps) then Q*abs(Q) else Q*eps;
  deltaP*Cv*abs(Cv) = 1.733e12*ThermoCar/rho^2;
  
  /* Calcul du Cv en fonction de l'ouverture */
  Cv = Ouv*Cvmax;
  
  /* Calcul des propriétés thermodynamiques */
  Pm = (C1.P + C2.P)/2;
  Hm = (C1.H + C2.H)/2;
  
  Tm = Hm/4187 + 273.15;
  region = 1;
  
  rho = 998;

end VanneReglante;
