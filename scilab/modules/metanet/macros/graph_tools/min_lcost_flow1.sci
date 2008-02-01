function [c,phi,flag]=min_lcost_flow1(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// check g
check_graph(g)
// check capacities
ma=prod(size(g('tail')))
n=g('node_number')
mincap=g('edge_min_cap')
maxcap=g('edge_max_cap')
if mincap==[] then
  mincap=zeros(1,ma)
end
if maxcap==[] then
  maxcap=zeros(1,ma)
end
verif=find(mincap<0) 
if verif<>[] then 
  error('Minimum capacities must be non negative')
end
verif=find(maxcap<0) 
if verif<>[] then 
  error('Maximum capacities must be non negative')
end
verif=find(mincap>maxcap)
if verif<>[] then
  error('Maximum capacities must be greater than minimal capacities')
end
if or(mincap<>round(mincap)) then
  error('Minimum capacities must be integer')
end
if or(maxcap<>round(maxcap)) then
  error('Maximum capacities must be integer')
end
// check costs
costs=g('edge_cost')
if costs==[] then
  costs=zeros(1,ma)
end
verif=find(costs<0) 
if verif<>[] then 
  error('Costs must be non negative')
end
// check demand
demand=g('node_demand')
if demand==[] then
  demand=zeros(1,n)
end
verif=find(demand<>0)
if verif<>[] then 
  error('Demands must be equal to zero')
end
// compute lp, la and ln
// always consider the graph as undirected!
[lp,la,ln]=m6ta2lpu(g('tail'),g('head'),n+1,n,2*ma)
// compute linear min cost flow by out-of-kilter algorithm
[phi,flag]=m6kilter(mincap,maxcap,g('head'),g('tail'),la,lp,n,costs)
c=costs*phi'
endfunction
