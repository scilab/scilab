function [p,r]=circuit(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// check g
check_graph(g)
// compute lp, la and ln
n=g('node_number')
ma=prod(size(g('tail')))
if g('directed')==1 then
  [lp,la,ln]=m6ta2lpd(g('tail'),g('head'),n+1,n)
else
  error('The graph must be directed')
end
// compute rank function
[i,r]=m6frang(lp,ln,n)
if i==0 then p=[]
else p=m6prevn2p(i,i,r,la,lp,ln,g('directed')), r=[]
end
endfunction
