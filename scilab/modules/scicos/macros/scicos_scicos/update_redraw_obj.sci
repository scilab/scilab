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

function scs_m = update_redraw_obj(scs_m, path, o)
//** 02/12/06 : use of objects permutation in gh_curwin.children.children()

if length(path)==2 then
  //** --------- LINK   and -------- TEXT --------------
  if typeof(o)=="Link" | typeof(o)=="Text" then
    //** Link and Text are update immediately here 
    scs_m(path) = o ; 
    //** quick update for new graphics
    drawlater() ;
     gh_curwin = scf(gh_current_window);
     gh_axes = gca(); 
     o_size = size(gh_axes.children);
     gr_k = get_gri(path(2), o_size(1)) ; //** semi empirical equation :)
     update_gr(gr_k, o) //** update the graphics data structure only of the last object 
     //** draw(gh_curwin.children); //** update the graphics window  //** show the diagram 
     drawnow(); 
     //** show_pixmap() ; 
  else
    //** ---------------- BLOCK ------------------------
    //** the BLOCK and connected links are updated here 
    //** new graphics functions is now included in changeports
    scs_m = changeports(scs_m, path, o)
  end
  
else // change a block in a sub-level

  scs_m(path) = o ;

end

endfunction
