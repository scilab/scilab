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

model SineVoltage "Sine voltage source" 
  parameter Real V=1 "Amplitude of sine wave";
  parameter Real phase=0 "Phase of sine wave";
  parameter Real freqHz=1 "Frequency of sine wave";
  parameter Real offset=0 "Offset volatge of sine wave";
  parameter Real startTime=0 "sine wave start time";
  parameter Real PI=3.141592653589793;
  Pin p, n;
  Real v;
  
equation

  v = offset + (if time < startTime then 0 else V*Modelica.Math.sin(2*PI*freqHz*(time - startTime) +phase));
  v= p.v - n.v;	
  0 = p.i + n.i;
end SineVoltage;


