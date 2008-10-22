function [m]=arc_number(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
ma=prod(size(g('tail')))
if g('directed')==1 then m=ma, else m=2*ma, end
endfunction
