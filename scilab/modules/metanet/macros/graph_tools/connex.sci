function [nc,ncomp]=connex(g)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // check g
  check_graph(g,%f)
  // compute lp and ln
  ma=size(g.edges);
  n=node_number(g)
  if g.directed then
    // if the graph is directed, get the corresponding undirected one
    [lp,la,ln]=m6ta2lpd([g.edges.tail, g.edges.head],..
			[g.edges.head, g.edges.tail],..
			n+1,n)
  else
    [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  end
  // compute connexity
  [nc,ncomp]=m6compc(lp,ln,n)
endfunction
