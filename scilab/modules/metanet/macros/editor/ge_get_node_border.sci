function thick=ge_get_node_border(n)
  if GraphList.nodes.graphics.border==[] then
    thick=GraphList.nodes.graphics.defaults.border
  else
    thick=GraphList.nodes.graphics.border(n)
    if thick==0 then thick=GraphList.nodes.graphics.defaults.border,end
  end
endfunction
