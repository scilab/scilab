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

class PMOS "Simple MOS Transistor" 
  parameter Real W=50.0e-6 "Width";
  parameter Real L=6.0e-6 "Length";
  parameter Real Beta=0.0105e-3 "Transconductance parameter";
  parameter Real Vt=-1 "Zero bias threshold voltage";
  parameter Real K2=0.41 "Bulk threshold parameter";
  parameter Real K5=0.839 "Reduction of pinch-off region";
  parameter Real dW=-2.5e-6 "Narrowing of channel";
  parameter Real dL=-2.1e-6 "Shortening of channel";
  parameter Real RDS=1.e+7 "Drain-Source-Resistance";

  Pin D "Drain";
  Pin G "Gate";
  Pin S "Source";
  Pin B "Bulk";

  Real v;
  Real uds;
  Real ubs;
  Real ugst;
  Real ud;
  Real us;
  Real id,gds;
equation 
  gds = 1/RDS;//if (RDS < 1.e-20 and RDS > -1.e-20) then 1.e20 else 1/RDS;
  v = Beta*(W + dW)/(L + dL);
  ud = if noEvent(D.v > S.v) then S.v else D.v;
  us = if noEvent(D.v > S.v) then D.v else S.v;
  uds = ud - us;
  ubs = if noEvent(B.v < us) then 0 else B.v - us;
  ugst = (G.v - us - Vt + K2*ubs)*K5;
  id = if noEvent(ugst >= 0) then uds*gds else if noEvent(ugst < uds) then -v*uds*(
    ugst - uds/2) + uds*gds else -v*ugst*ugst/2 + uds*gds;
  G.i = 0;
  D.i = if noEvent(D.v > S.v) then -id else id;
  S.i = -D.i;
  B.i = 0;
end PMOS;

