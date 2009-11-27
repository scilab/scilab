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

class Switch
  parameter Real Ron=0.01 "Resistance when the Switch is closed";
  parameter Real Roff=1e5  "Resistance when the switch is open";
  Pin p,n;
  Real inp,Rx;
equation
  Rx*p.i= p.v - n.v;
  p.i = -n.i;
  Rx=if inp >0 then Ron	
              else Roff;
end Switch;
