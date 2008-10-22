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

class PNP "Simple BJT according to Ebers-Moll" 
  parameter Real Bf=50 "Forward beta";
  parameter Real Br=0.1 "Reverse beta";
  parameter Real Is=1.e-16 "Transport saturation current";
  parameter Real Vak=0.02 "Early voltage (inverse), 1/Volt";
  parameter Real Tauf=0.12e-9 "Ideal forward transit time";
  parameter Real Taur=5e-9 "Ideal reverse transit time";
  parameter Real Ccs=1e-12 "Collector-substrat(ground) cap.";
  parameter Real Cje=0.4e-12 "Base-emitter zero bias depletion cap.";
  parameter Real Cjc=0.5e-12 "Base-coll. zero bias depletion cap.";
  parameter Real Phie=0.8 "Base-emitter diffusion voltage";
  parameter Real Me=0.4 "Base-emitter gradation exponent";
  parameter Real Phic=0.8 "Base-collector diffusion voltage";
  parameter Real Mc=0.333 "Base-collector gradation exponent";
  parameter Real Gbc=1e-15 "Base-collector conductance";
  parameter Real Gbe=1e-15 "Base-emitter conductance";
  parameter Real Vt=0.02585 "Voltage equivalent of temperature";
  parameter Real EMinMax=40 "if x < EMin, the exp(x) function is linearized";
  Real vbc;
  Real vbe;
  Real qbk;
  Real ibc;
  Real ibe;
  Real cbc;
  Real cbe;
  Real ExMin;
  Real ExMax;
  Real Capcje;
  Real Capcjc;
  Real EMax;
  Real EMin;

  Pin C "Collector";
  Pin B "Base";
  Pin E "Emitter";
equation 
  EMax=EMinMax;
  EMin=-2*EMinMax;
  ExMin = Modelica.Math.exp(EMin);
  ExMax = Modelica.Math.exp(EMax);
  vbc = C.v - B.v;
  vbe = E.v - B.v;
  qbk = 1 - vbc*Vak;
  
  ibc = if noEvent(vbc/Vt < EMin) then Is*(ExMin*(vbc/Vt - EMin + 1) - 1) + vbc*
    Gbc else if noEvent(vbc/Vt > EMax) then Is*(ExMax*(vbc/Vt - EMax + 1) - 1) + 
    vbc*Gbc else Is*(Modelica.Math.exp(vbc/Vt) - 1) + vbc*Gbc;
  
  ibe = if noEvent(vbe/Vt < EMin) then Is*(ExMin*(vbe/Vt - EMin + 1) - 1) + vbe*
    Gbe else if noEvent(vbe/Vt > EMax) then Is*(ExMax*(vbe/Vt - EMax + 1) - 1) + 
    vbe*Gbe else Is*(Modelica.Math.exp(vbe/Vt) - 1) + vbe*Gbe;
  
  Capcjc = if (vbc/Phic > 0) then Cjc*(1 + Mc*vbc/Phic) else Cjc*pow(1 - 
    vbc/Phic, -Mc);
  Capcje = if (vbe/Phie > 0) then Cje*(1 + Me*vbe/Phie) else Cje*pow(1 - 
    vbe/Phie, -Me);

  cbc = if noEvent(vbc/Vt < EMin) then Taur*Is/Vt*ExMin*(vbc/Vt - EMin + 1) + 
    Capcjc else if noEvent(vbc/Vt > EMax) then Taur*Is/Vt*ExMax*(vbc/Vt - EMax + 1)
     + Capcjc else Taur*Is/Vt*Modelica.Math.exp(vbc/Vt) + Capcjc;
  cbe = if noEvent(vbe/Vt < EMin) then Tauf*Is/Vt*ExMin*(vbe/Vt - EMin + 1) + 
    Capcje else if noEvent(vbe/Vt > EMax) then Tauf*Is/Vt*ExMax*(vbe/Vt - EMax + 1)
     + Capcje else Tauf*Is/Vt*Modelica.Math.exp(vbe/Vt) + Capcje;

  C.i = -((ibe - ibc)*qbk - ibc/Br - cbc*der(vbc) + Ccs*der(C.v));
  B.i = -(ibe/Bf + ibc/Br + cbe*der(vbe) + cbc*der(vbc));
  E.i = -B.i - C.i - Ccs*der(C.v);
end PNP;


