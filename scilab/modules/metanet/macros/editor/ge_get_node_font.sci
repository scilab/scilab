function font=ge_get_node_font(n)
  if GraphList.nodes.graphics.font==[] then
    font=GraphList.nodes.graphics.defaults.font*ones(1,size(n,'*'))
  else
    font=GraphList.nodes.graphics.font(:,n)
    for k=1:3
      font(k,font(k,:)==0)=GraphList.nodes.graphics.defaults.font(k)
    end
  end
endfunction

