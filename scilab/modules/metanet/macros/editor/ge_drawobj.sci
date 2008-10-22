
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function ge_drawobj(o)

  if typeof(o)=='node' then
    ge_draw_node(o)
  elseif typeof(o)=='arc' then
    ge_draw_arc(o)
  elseif typeof(o)=='Text' then
    ge_draw_text(o)
  end
endfunction
function ge_draw_node(o)
  d=o.node_diam
  x=o.node_x
  y=o.node_y
  xarc(x-d/2,y+d/2,d,d,0,360*64)
endfunction
function ge_draw_arc(o)
endfunction
