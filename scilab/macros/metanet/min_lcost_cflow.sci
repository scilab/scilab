function [c,phi,v,flag]=min_lcost_cflow(i,j,cv,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>4 then error(39), end
// check i and j
if prod(size(i))<>1 then
  error('First argument must be a scalar')
end
if prod(size(j))<>1 then
  error('Second argument must be a scalar')
end
// check cv
if prod(size(cv))<>1 then
  error('Third argument (constrained cost) must be a scalar')
end
if cv<0 then
  error('Constrained cost must be positive')
end
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
if g('directed')==1 then
  [lp,la,ln]=m6ta2lpd(g('tail'),g('head'),n+1,n)
else
  [lp,la,ln]=m6ta2lpu(g('tail'),g('head'),n+1,n,2*ma)
end
// compute constrained min cost flow by Busacker and Goven algorithm
[v,phi,flag]=m6busack(i,j,cv,maxcap,g('head'),g('tail'),la,lp,n,costs)
c=costs*phi'
endfunction
