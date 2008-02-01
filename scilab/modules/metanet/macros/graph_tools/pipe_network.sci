function [x,pi]=pipe_network(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end 
// g
check_graph(g)
if g('directed') <> 1 then
  error('The graph must be directed')
end
// check weights
r=g('edge_weight');
if max(r)<=0 then
  error('Weights (resistances) must be strictly positive')
end
// check demands
n=g('node_number');
ma=edge_number(g);
demand=g('node_demand');
if demand==[] then
  demand=zeros(1,n)
end
if demand*ones(n,1)<>0 then
  error('The problem is not feasible')
end

a=graph_2_mat(g);
rp=sparse([[1:ma]' [1:ma]'],1 ./ r);
ap=a(1:$-1,:);
A=ap*rp*ap';
B=demand(1:$-1)';
[h,rk]=lufact(A);
pi=lusolve(h,B); pi(n)=0;
ludel(h);
x=-rp*a'*pi;
endfunction
