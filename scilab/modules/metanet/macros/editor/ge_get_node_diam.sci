function diam=ge_get_node_diam(n)
  if GraphList.nodes.graphics.diam==[] then
    diam=GraphList.nodes.graphics.defaults.diam(1,ones(n))
  else
    diam=GraphList.nodes.graphics.diam(n)
    diam(diam==0)=GraphList.nodes.graphics.defaults.diam
  end
endfunction
