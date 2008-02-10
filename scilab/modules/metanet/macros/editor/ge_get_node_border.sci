function thick=ge_get_node_border(n)
  if GraphList.nodes.graphics.border==[] then
    thick=GraphList.nodes.graphics.defaults.border(1,ones(n))
  else
    thick=GraphList.nodes.graphics.border(n)
    thick(thick==0)=GraphList.nodes.graphics.defaults.border;
  end
endfunction
