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

function sz=getportsiz(o,prt_number,typ)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  sz=[]
  typs=['in','out','evtin','evtout']
  k=find(typ==typs)
  if k==[] then return,end
  select k
   case 1 then
     in=o.model.in(prt_number)
     if size(o.model.in2,'*')==0|...
        size(o.model.in2,'*')>=size(o.model.in,'*') then
       in2=o.model.in2(prt_number)
     else
       in2=1
     end
     sz=[in in2]
   case 2 then
     out=o.model.out(prt_number)
     if size(o.model.out2,'*')==0|...
        size(o.model.out2,'*')>=size(o.model.out2,'*') then
       out2= o.model.out2(prt_number)
     else
       out2=1
     end
     sz=[out out2]
   case 3 then
     psiz=o.model(typs(k))
     sz=psiz(prt_number)
   case 4 then
     psiz=o.model(typs(k))
     sz=psiz(prt_number)
  end
endfunction
