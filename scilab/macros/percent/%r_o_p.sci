function r=%r_o_p(l1,l2)
// l1==l2  with l1 rational and  l2 polynomial
//!
// Copyright INRIA
r=degree(l1('den'))==0
if r then r=l1('num')./coeff(l1('den'))==l2,end
endfunction
