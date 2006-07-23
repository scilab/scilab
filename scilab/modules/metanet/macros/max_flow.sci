function [v,phi,ierr]=max_flow(i,j,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>3 then error(39), end
// check i and j
if prod(size(i))<>1 then
  error('First argument must be a scalar')
end
if prod(size(j))<>1 then
  error('Second argument must be a scalar')
end
if (i == j) then
  error('Source and sink must not be the same node')
end
// check g
check_graph(g)
if g('directed') <> 1 then
  error('The graph must be directed')
end
ma=prod(size(g('tail')))
n=g('node_number')
if ( (i > n) | (i < 0) ) then
  error('i is not a node number')
end
if ( (j > n) | (j < 0) ) then
  error('i is not a node number')
end
// check capacities
mincap=g('edge_min_cap')
maxcap=g('edge_max_cap')
if mincap==[] then
  mincap=zeros(1,ma)
end
if maxcap==[] then
  maxcap=zeros(1,ma)
end
ldif=find(mincap>maxcap)
if ldif<>[] then
  error('Maximum capacities must be greater than minimal capacities')
end
// compute maximum flow
[v,phi,ierr]=m6fordfulk(n,ma,i,j,maxcap,g('tail'),g('head'))
endfunction
