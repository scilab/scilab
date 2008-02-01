function diam=ge_get_node_diam(n)
  if GraphList.nodes.graphics.diam==[] then
    diam=GraphList.nodes.graphics.defaults.diam
  else
    diam=GraphList.nodes.graphics.diam(n)
    if diam==0 then diam=GraphList.nodes.graphics.defaults.diam,end
  end
endfunction
