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

function disablemenus(xinfo_flag)
//DisableAllMenus()
return
//**   

//** Disable the dynamic menu to avoid unintentional command generation during
//** interactive operations (Move, OpenSet, Link creation, etc.)

//** if a "xinfo_flag" is specified, the default xinfo message is not printed 

//** [lhs,rhs] = argn()

if rhs<1 then //** if no argument is specified
  xinfo("Please be patient...") ; //** print the default message 
end   

//   curwin=xget('window') //** Please left this instruction commented 
//** if rhs<1 then         //** obsolete code inside 
//**   men = menus(1)
//** end

men = menus(1); 
for k=1:size(men,'*')
  unsetmenu(curwin,men(k)); //** 
end

endfunction
