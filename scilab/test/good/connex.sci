function [nc,ncomp]=connex(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// check g
check_graph(g)
// compute lp and ln
ma=prod(size(g('tail')))
n=g('node_number')
if g('directed') == 1 then
  // if the graph is directed, get the corresponding undirected one
  [lp,la,ln]=m6ta2lpd(..
      [matrix(g('tail'),1,ma),matrix(g('head'),1,ma)],..
      [matrix(g('head'),1,ma),matrix(g('tail'),1,ma)],..
      n+1,n)
else
  [lp,la,ln]=m6ta2lpu(g('tail'),g('head'),n+1,n,2*ma)
end
// compute connexity
[nc,ncomp]=m6compc(lp,ln,n)
endfunction
