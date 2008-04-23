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

