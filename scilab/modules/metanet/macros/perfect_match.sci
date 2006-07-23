function [cost,nmatch]=perfect_match(g,arcost)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// check g
if g('directed')==1 then
  error('The graph must be undirected')
end
check_graph(g)
m=prod(size(g('tail')));m2=2*m;
n=g('node_number');
ii=n-2*round(n/2)
if (ii <> 0) then
  error('The number of nodes must be even')
end
ii=size(arcost,2);
if (ii <> m2) then
  error('Bad dimension for ""arcost""')
end
[lp,la,ln]=m6ta2lpu(g('tail'),g('head'),n+1,n,m2)
[cost,nmatch]=m6prfmatch(n,m,n+1,m2,ln,arcost,lp);
endfunction
