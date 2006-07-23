function [c,phi,flag]=min_qcost_flow(eps,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// check eps
if prod(size(eps))<>1 then
  error('First argument must be a scalar')
end
if eps<=0 then
  error('First argument must be strictly positive')  
end
p=-log(eps)/log(2)
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
verif=find(mincap>maxcap)
if verif<>[] then
  error('Maximum capacities must be greater than minimal capacities')
end
verif=find(mincap<>maxcap)
if verif==[] then
  error('Maximum capacities must not be all equal to minimal capacities')
end
// check quadratic costs
qorig=g('edge_q_orig')
qweight=g('edge_q_weight')
if qorig==[] then
  qorig=zeros(1,ma)
end
if qweight==[] then
  qweight=zeros(1,ma)
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
// compute min quadratic cost flow
[phi,flag]=m6floqua(p,mincap,maxcap,g('head'),g('tail'),la,lp,n,qorig,qweight)
c=sum(qweight.*(phi-qorig).*(phi-qorig))/2
endfunction
