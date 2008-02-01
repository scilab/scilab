function g=graph_sum(g,g1)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check g and g1
  check_graph(g)
  check_graph(g1)
  n=node_number(g); 
  n1=node_number(g1);
  if (n1 <> n) then
    error('The two graphs must have the same number of nodes')
  end

  // sum of 2 graphs (sum of adjacency matrices)
  
  Fe=getfield(1,g.edges);
  for f=Fe(2:$)
    g.edges(f)=[g.edges(f) g1.edges(f)];
  end

endfunction
