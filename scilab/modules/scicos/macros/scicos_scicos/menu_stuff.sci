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

function menu_stuff()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


//** Remove the default Scilab graphics window menus 
delmenu(curwin,_("&File"))  ; 
delmenu(curwin,_("&Edit"))  ;
delmenu(curwin,_("&Tools")) ;
delmenu(curwin,_("&?"))     ; 

//** Remove the toolbar 
toolbar(curwin, "off"); //** by Vincent C. :)

//** Add the Scicos specific menu 
menuss    = menus        ;
menuss(1) = menus(1)(2:$);
scicos_menubar(curwin,menuss)   ; //** this function add physically the Scicos menus

endfunction
