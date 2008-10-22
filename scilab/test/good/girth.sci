function [d]=girth(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// check g
if g('directed')<>1 then
  error('The graph must be directed')
end
// girth of graph g (length of the shortest cycle)
[p,r] = circuit(g);
if p==[] then 
   d=0
end
l=size(p,2);
d=l;
ta=g('tail');he=g('head');
n=g('node_number'); 
X=sparse([ta' he'],ones(ta)',[n n]);Y=X;
for i=2:(l-1),
  Y=Y*X;
  if sum(diag(Y))<>0 then
    d=i;
  end
end
endfunction
