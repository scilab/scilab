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

function [scs_m,edited,ok]=do_rename(scs_m, pal_mode)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  ok=%t
  if argn(2)==1 then pal_mode=%f, end

  if pal_mode then
    mess='Enter the new palette name'
  else
    mess='Enter the new diagram name'
  end
  
  [ok,new]=scicos_getvalue(mess,"Name",list("str",[1,1]),scs_m.props.title(1))
  
  if new<>[] then
    drawtitle(scs_m.props)     //erase title
    scs_m.props.title(1) = new 
    drawtitle(scs_m.props)     //draw title
    edited = %t
  end
endfunction
