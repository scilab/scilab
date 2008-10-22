
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
  if (Id == []) then Id = ''; end

  // modify default settings if needed
  a = gca();
  if (a.font_foreground <> font(3)) then
    a.font_foreground = font(3)
  end
  if (a.font_style <> font(2)) then
    a.font_style = font(2)
  end
  if (a.font_size <> font(1)) then
    a.font_size = font(1)
  end

  xstring(0,0,Id);h=gce()
  if Id<>'' then
    r = stringbox(h);
    width = abs(r(1,3) - r(1,1)) / 2;
    height = abs(r(2,3) - r(2,1)) / 2;
    select loc
     case 'center'
      h.data=[node_x-width,node_y-height]
     case 'right'
      h.data=[node_x+node_w/2,node_y-height]
     case 'left'
      h.data=[node_x-node_w/2- 2 * width,node_y-height]
     case 'up' 
      h.data=[node_x-width,node_y+node_h/2+height]
     case 'down'
      h.data=[node_x-width,node_y-node_h/2-3*height]
    end
    //h.visible='on'
  end
endfunction
