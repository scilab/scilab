
function ge_drawobj(o)
//Copyright INRIA
//Author : Serge Steer 2002

// Copyright INRIA

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
