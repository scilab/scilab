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

function sztyp=getporttyp(o,prt_number,typ)
  sztyp=[]
  typs=['in','out']
  k=find(typ==typs)
  if k==[] then return,end

  select k
   case 1 then
     if size(o.model.intyp,'*')<prt_number then //for compatibilty
      sztyp=1;
     else
      sztyp=o.model.intyp(prt_number);
     end
   case 2 then
     if size(o.model.outtyp,'*')<prt_number then //for compatibilty
      sztyp=1;
     else
      sztyp=o.model.outtyp(prt_number)
     end
  end
endfunction
