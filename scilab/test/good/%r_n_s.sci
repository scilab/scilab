function [r]=%r_n_s(l1,l2)
// l1<>l2     rational<>constant
// Copyright INRIA
r=degree(l1('num'))==0&degree(l1('den'))==0
if r then r=coeff(l1('num'))./coeff(l1('den'))==l2,end
r=~r
endfunction
