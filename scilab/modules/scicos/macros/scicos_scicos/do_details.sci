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

  if type(x)==1 then
    Select = x ;
    sel_items = size(Select)   ; 
    obj_selected = sel_items(1) ; 
    if obj_selected==0 then
      o = scs_m;
    else
      cwin = Select(1,2)
      if cwin==curwin then
	k = Select(1,1)
	o = scs_m.objs(k)
      elseif or(windows(find(windows(:,1)<0),2)==cwin) then
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
