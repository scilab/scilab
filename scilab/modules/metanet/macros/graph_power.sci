function [g1]=graph_power(g,k)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// check g
check_graph(g)
if g('directed')<>1 then
  error('The graph must be directed')
end
// check k
if k<1 then
  error('Power must be greater than 0')
end
if k==1 then
  g1=g; return;
end
//graph power
ta=g('tail');he=g('head');
n=g('node_number'); 
X=sparse([ta' he'],ones(ta)',[n n]);Y=X;Z=X;
for i=2:k,
  Y=Y*X;
  [ij,v,mn]=spget(Y);
  if (v <> []) then 
    Z=Z+Y;i=k;
  end;
end;
[ij,v,mn]=spget(Z);
ta=[ij(:,1)'];he=[ij(:,2)'];
g1=make_graph('foo',1,n,ta,he);
g1('node_x')=g('node_x');g1('node_y')=g('node_y');
endfunction
