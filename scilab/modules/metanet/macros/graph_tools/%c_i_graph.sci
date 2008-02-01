function g=%c_i_graph(i,d,g)
//called for compatibility g.node_x, g.node_y g.edge_color
  if  i=='node_name' then
    g.nodes.graphics.name=d
  elseif i=='edge_label' then
    F=getfield(1,g.edges.data)
    if and(F<>'label') then 
      data=g.edges.data
      setfield(1,[F 'label'],data)
      data.label=emptystr(1,edge_number(g))
      g.edges.data=data
    end
    g.edges.data.label=d
  end
endfunction
