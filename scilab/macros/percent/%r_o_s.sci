function r=%r_o_s(l1,l2)
// l1==l2 with l1 rational and l2 constant
//!
// Copyright INRIA
r=degree(l1('num'))==0&degree(l1('den'))==0
if r then r=coeff(l1('num'))./coeff(l1('den'))==l2,end
endfunction
