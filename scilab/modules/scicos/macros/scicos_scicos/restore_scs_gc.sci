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

function restore_scs_gc(scs_gc)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// restore the current scicos graphic context
  xset('window',scs_gc(1))
xsetech(scs_gc(2)(1),scs_gc(2)(2))
driver(scs_gc(3))
for i=4:size(scs_gc)
  x=scs_gc(i)
  xx=x(2);
  select size(x(2),'*'),
  case 1 then 
    xset(x(1),xx);
  case 2 then 
    xset(x(1),xx(1),xx(2));
  case 3 then 
    xset(x(1),xx(1),xx(2),xx(3));
  case 4 then 
    xset(x(1),xx(1),xx(2),xx(3),xx(4));
  case 5 then 
    xset(x(1),xx(1),xx(2),xx(3),xx(4),xx(5));
  end
end
endfunction
