function g=delete_nodes(del_nodes,g)
// Copyright INRIA
  if argn(2)<>2 then error(39), end

  if del_nodes==[] then  return; end;
  del_nodes=unique(del_nodes)
  if del_nodes(1)<1|del_nodes($)>node_number(g) then
    error('A number in first argument is not a node number')
  end
  
  //connected arcs
  del_arcs=find(dsearch(g.edges.tail,del_nodes,'d')>0|..
		dsearch(g.edges.head,del_nodes,'d')>0)
  g=ge_delete_node(g,del_nodes)
  g.edges(del_arcs)=[]
endfunction
