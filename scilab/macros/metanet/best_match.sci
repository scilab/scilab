function [card,match]=best_match(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// check graph
if g('directed')==1 then
  error('The graph must be undirected')
end
check_graph(g)
m=prod(size(g('tail')));
n=g('node_number');
nd2=n/2;nd8=int(n/8);np1=n+1;nt2=2*n;mt2=2*m;mt3=3*m;mt4=4*m;
n8t8=ceil(n/8)*8;
ta=g('tail');he=g('head');
[card,match]=m6bmatch(n,m,nd2,n8t8,nd8,np1,nt2,mt2,mt3,mt4,n,m,ta,he)
endfunction
