function y=erfc(x)
//erfc  -  The complementary error function.
//%Syntax
// y = erf(x).
//%Parameters
// x : real vector
// y : real vector (of same size)
//%Description
// erf computes the complementary error function:
//
//                      /inf
//      y = 2/sqrt(pi) *|  exp(-t^2) dt
//                      /x
//      y = 1 - erf(x)
//%See also 
// erf erfcx calerf
//!
// Copyright INRIA
y=calerf(x,1)
endfunction
