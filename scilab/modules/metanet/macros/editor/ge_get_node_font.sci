function font=ge_get_node_font(n)
  if GraphList.nodes.graphics.font==[] then
    font=GraphList.nodes.graphics.defaults.font
  else
    font=GraphList.nodes.graphics.font(:,n)
    font(font==0)=GraphList.nodes.graphics.defaults.font(font==0)
  end
endfunction

