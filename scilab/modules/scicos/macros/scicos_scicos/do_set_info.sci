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
// This function may be redefined by the user to handle definition 
// of the informations associated with the current diagram

if prod(size(info))==0 then
  info = list(' ')
end

new_info = x_dialog('Set Diagram informations',info(1))

if new_info==[] then 
  ok = %f
else
  ok = %t
  new_info = list(new_info)
end

endfunction
