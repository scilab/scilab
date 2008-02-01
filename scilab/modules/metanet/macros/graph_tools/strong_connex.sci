function [nc,ncomp]=strong_connex(g)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  // g
  check_graph(g,%f)
  // compute lp and ln
  ma=prod(size(g.edges.tail))
  n=node_number(g);
  if g.directed == 1 then
    [lp,la,ln]=m6ta2lpd(g.edges.tail,g.edges.head,n+1,n)
  else
    [lp,la,ln]=m6ta2lpu(g.edges.tail,g.edges.head,n+1,n,2*ma)
  end
  // compute connexity
  [nc,ncomp]=m6compfc(lp,ln,n)
endfunction
