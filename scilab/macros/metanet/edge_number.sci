function [ma]=edge_number(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
ma=prod(size(g('tail')))
endfunction
