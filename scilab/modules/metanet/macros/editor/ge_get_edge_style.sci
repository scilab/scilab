function [thick,col,style]=ge_get_edge_style(n)
  if GraphList.edges.graphics.width==[] then
    thick=GraphList.edges.graphics.defaults.width
  else
    thick=GraphList.edges.graphics.width(n)
    if thick==0 then thick=GraphList.edges.graphics.defaults.width,end
  end
  if GraphList.edges.graphics.foreground==[] then
    col=GraphList.edges.graphics.defaults.foreground
  else
    col=GraphList.edges.graphics.foreground(n)
    if col==0 then col=GraphList.edges.graphics.defaults.foreground,end
  end
  style=0
endfunction
