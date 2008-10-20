function [c,phi,flag]=min_lcost_flow2(g)
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
verif=find(mincap<>0)
if verif<>[] then 
  error('Minimum capacities must be equal to zero')
end
verif=find(maxcap<0) 
if verif<>[] then 
  error('Maximum capacities must be non negative')
end
if or(maxcap<>round(maxcap)) then
  error('Maximum capacities must be integer')
end
// check costs
costs=g('edge_cost')
if costs==[] then
  costs=zeros(1,ma)
end
if or(costs<>round(costs)) then
  error('Costs must be integer')
end
// check demand
demand=g('node_demand')
if demand==[] then
  demand=zeros(1,n)
end
if or(demand<>round(demand)) then
  error('Demands must be integer')
end
if sum(demand)<>0 then
  error('Sum of demands must be equal to zero')
end
// compute linear min cost flow by relaxation method (Bertsekas)
[c,phi,flag]=m6relax(g('head'),g('tail'),costs,maxcap,demand,arc_number(g),n)
endfunction
