function n=node_number(g)
//returns the number of nodes in a graph
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  n=size(g.nodes.graphics.x,'*');
endfunction
