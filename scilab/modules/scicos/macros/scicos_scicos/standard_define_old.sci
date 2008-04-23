//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function o = standard_define_old(sz, model, label, gr_i)
//initialize graphic part of the block data structure
//**    
[lhs,rhs]=argn(0)
if rhs<4 then gr_i=[],end
[nin,nout,ncin,ncout]=model(2:5)
nin=size(nin,1);if nin>0 then pin(nin,1)=0,else pin=[],end
nout=size(nout,1);if nout>0 then pout(nout,1)=0,else pout=[],end
ncin=size(ncin,1);if ncin>0 then pcin(ncin,1)=0,else pcin=[],end
ncout=size(ncout,1);if ncout>0 then pcout(ncout,1)=0,else pcout=[],end
graphics=list([0,0],sz,%t,label,pin,pout,pcin,pcout,gr_i)
if model(1)(1)=='super' then
   o=list('Block',graphics,model,' ','SUPER_f')
else
  [ln,mc]=where()
  o=list('Block',graphics,model,' ',mc(2))
end

endfunction

