function cols=ge_get_node_colors(n)
  cols=GraphList.nodes.graphics.colors(:,n)
  if or(cols==0) then
    cols(cols==0)=GraphList.nodes.graphics.defaults.colors(cols==0)
  end
endfunction
