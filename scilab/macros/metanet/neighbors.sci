function [d]=neighbors(i,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
n=g('node_number');
// check i
if (i<1|i>n) then
  error(string(i)+' is not a node number')
end
ta=g('tail');he=g('head');
[ir,ic]=find(ta==i);
d1=he(ic);
[ir,ic]=find(he==i);
d2=ta(ic);
d=-sort(-[d1 d2]);
endfunction
