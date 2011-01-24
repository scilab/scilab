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

function [ok,new_info]=do_set_info(info)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// This function may be redefined by the user to handle definition 
// of the information associated with the current diagram

if prod(size(info))==0 then
  info = list(' ')
end

//new_info = x_dialog('Set Diagram information',info(1))

//## set param of scstxtedit
ptxtedit=scicos_txtedit(clos = 0,...
          typ  = "scsminfo",...
          head = ['Set Diagram information']);

while 1==1

  [txt,Quit] = scstxtedit(info(1),ptxtedit);

  if ptxtedit.clos==1 then
    break;
  end

  if txt==[]|Quit==1 then
    new_info=[]
    ok = %f
    break;
  else
    ok = %t
    new_info = list(txt)
    ptxtedit.clos=1
  end

end

endfunction

