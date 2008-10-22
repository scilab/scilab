function [r]=%p_o_r(l1,l2)
//r=%p_o_r(l1,l2) <=> r=(l1==l2) l1 polynomial  l2 rational
//!
// Copyright INRIA
r=degree(l2('den'))==0
if r then r=l2('num')./coeff(l2('den'))==l1,end
endfunction
