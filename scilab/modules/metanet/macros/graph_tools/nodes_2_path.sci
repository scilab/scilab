function p=nodes_2_path(ns,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// check ns
s=size(ns)
if s(1)*s(2) == 0 then p=[]; return end
if s(1)<>1 then
  error('First argument must be a row vector')
end
if (s(1)*s(2)==1) then
  error('First argument must be a vector with at least 2 elements')
end
// check g
check_graph(g)
// compute lp, la and ln
ma=prod(size(g('tail')))
n=g('node_number')
if g('directed') == 1 then
  [lp,la,ln]=m6ta2lpd(g('tail'),g('head'),n+1,n)
else
  [lp,la,ln]=m6ta2lpu(g('tail'),g('head'),n+1,n,2*ma)
end
// compute path
p=m6ns2p(ns,la,lp,ln,n)
endfunction
