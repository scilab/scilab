
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function h=ge_set_node_id(Id,font,loc,node_x,node_y,node_w,node_h)
//sets the string entity properties and compute its position
//h : handle on the string entity
  xstring(0,0,"");h=gce()
  h.font_foreground=font(3)
  h.font_style=font(2)
  h.font_size=font(1)
  if Id<>[] then 
    r=xstringl(0,0,Id,h.font_style,font(1))
    select loc
     case 'center'
      h.data=[node_x-r(3)/2,node_y-r(4)/2]
     case 'right'
      h.data=[node_x+node_w/2,node_y-r(4)/2]
     case 'left'
      h.data=[node_x-node_w/2-r(3),node_y-r(4)/2]
     case 'up' 
      h.data=[node_x-r(3)/2,node_y+node_h/2+r(4)/2]
     case 'down'
      h.data=[node_x-r(3)/2,node_y-node_h/2-3*r(4)/2]
    end	    
    h.text=Id
    h.visible='on'
  end
endfunction
