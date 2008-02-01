function d=successors(i,g)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<>2 then error(39), end
  // check g
  check_graph(g,%f)
  if g.directed<>1 then
    error('The graph must be directed')
  end
  n=node_number(g);
  // check i
  if (i<1|i>n) then
    error(string(i)+' is not a node number')
  end
  ta=g.edges.tail;he=g.edges.head;
  [ir,ic]=find(ta==i);
  d=he(ic);
endfunction
