function cols=ge_get_node_colors(n)
  if GraphList.nodes.graphics.colors==[] then
    cols=GraphList.nodes.graphics.defaults.colors*ones(1,size(n,'*'))
  else
    cols=GraphList.nodes.graphics.colors(:,n)
    for k=1:2
      cols(k,cols(k,:)==0)=GraphList.nodes.graphics.defaults.colors(k)
    end
  end
endfunction
