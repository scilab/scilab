function font=ge_get_edge_font(n)
  if GraphList.edges.graphics.font==[] then
    font=GraphList.edges.graphics.defaults.font
  else
    font=GraphList.edges.graphics.font(:,n)
    font(font==0)=GraphList.edges.graphics.defaults.font(font==0)
  end
endfunction
