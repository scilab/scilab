function ge_do_salesman(GraphList)
//Copyright INRIA
//Author : Serge Steer 2008
// Copyright INRIA
  if GraphList.directed then
    cir = salesman(GraphList);
  else
    ta=GraphList.edges.tail;
    he=GraphList.edges.head;
    g1=make_graph('foo1',1,node_number(GraphList),[ta he],[he ta]);
    cir = salesman(g1)
    ii=find(cir > edge_number(GraphList)); 
    if(ii <> []) then cir(ii)=cir(ii)-edge_number(GraphList);end;
  end
  show_arcs(cir,'sup');
endfunction

