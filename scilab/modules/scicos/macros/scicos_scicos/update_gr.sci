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

function update_gr(gr_k, o)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

 //** 20/02/2008 : This nice function, initially written by Alan Layec, has been
 //**              changed - just a bit - by Simone Mannori for Scilab 5 integration
 //**              Enjoy :)    

  //** 
  gh_blk = drawobj(o) ; //** new object in gh_curwin.children.children(1)
  
  //** This line is OK : export the Select information to the new object  
  gh_blk.children(1).mark_mode = gh_axes.children(gr_k+1).children(1).mark_mode;
  
   //** update the object in the old position
  //**               destination              source 
  swap_handles(gh_axes.children(gr_k+1), gh_axes.children(1));
  
  delete(gh_axes.children(1)); //** remove the object top of stack :) 

endfunction
