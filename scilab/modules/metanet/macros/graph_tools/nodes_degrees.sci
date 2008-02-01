function [outdegree,indegree]=nodes_degrees(g)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // check g
  check_graph(g,%f)

  ta=g.edges.tail;he=g.edges.head;n=node_number(g);np1=n+1;
  i=g.directed
  [lp,la,ln] = adj_lists(i,n,ta,he);
  outdegree=lp(2:np1)-lp(1:n);
  [lp,la,ln] = adj_lists(i,n,he,ta);
  indegree=lp(2:np1)-lp(1:n);
endfunction
