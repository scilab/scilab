function [r]=%s_o_r(l1,l2)
//%s_o_r(l1,l2) constant==rational
//!
// Copyright INRIA
r=degree(l2('num'))==0&degree(l2('den'))==0
if r then r=coeff(l2('num'))./coeff(l2('den'))==l1,end
endfunction
