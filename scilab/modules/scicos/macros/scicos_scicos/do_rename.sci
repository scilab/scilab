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

function [scs_m,edited]=do_rename(scs_m)
  if pal_mode then
    mess='Enter the new palette name'
  else
    mess='Enter the new diagram name'
  end

  new = dialog(mess,scs_m.props.title(1))
  new = new(1)
  
  if new<>[] then
    drawtitle(scs_m.props)     //erase title
    scs_m.props.title(1) = new 
    drawtitle(scs_m.props)     //draw title
    edited = %t
  end
endfunction
