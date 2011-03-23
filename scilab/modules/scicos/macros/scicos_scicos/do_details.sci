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

function do_details(x)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

//** 26/06/2009, Serge Steer: multiple selection detection
  if type(x)==1 then
    Select = x ;
    if Select==[] then // nothing selected return details on current diagram
      o = scs_m;
    elseif size(Select,1)>1 then
      messagebox(_("Multiple selection cannot be used here"),"error","modal")
      return
    else //a single object is selected
      cwin = Select(1,2)
      if cwin==curwin then //a block in current diagram
	k = Select(1,1)
	o = scs_m.objs(k)
      elseif or(windows(find(windows(:,1)<0),2)==cwin) then //a block in a palette
	kwin = find(windows(:,2)==cwin)   ;
	pal  = palettes(-windows(kwin,1)) ;
	k = Select(1,1) ;
	o = pal.objs(k);
      end
    end
  else
    o = x ; 
  end
  tree_show(o) ; //** beware to the BWidget version ! 

endfunction
