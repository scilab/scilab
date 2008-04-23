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

function enablemenus(men)
//EnableAllMenus()
return

[lhs,rhs] = argn()

//** check if the current window is in the Scicos windows list 
if ~or(curwin==winsid()) then
  return
end

//curwin=xget('window') //** Please left this instruction commented 
//** if rhs<1 then      //** obsolete code : is never used 
//**   men = menus(1)
//** end

men = menus(1) ;
for k=1:size(men,'*')
  setmenu(curwin,men(k)); //** reactivate the dynamic menu 
end

//** ... for the Super Block Diagram do NOT reactivate the "Simulate" menu
if super_block then
  unsetmenu(curwin,'Simulate');
end

xinfo(" "); 

endfunction
