function [r]=%r_n_p(l1,l2)
// l1<>l2  
// Copyright INRIA
r=degree(l1('den'))==0
if r then r=l1('num')./coeff(l1('den'))==l2,end
r=~r
endfunction
