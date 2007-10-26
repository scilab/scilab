function y=erf(x)
//erf  -  The error function.
//%Syntax
// y = erf(x).
//%Parameters
// x : real vector
// y : real vector (of same size)
//%Description
// erf computes the error function:
//
//                      /x
//      y = 2/sqrt(pi) *|  exp(-t^2) dt
//                      /0
//%See also 
// erfc erfcx calerf
//!
// Copyright INRIA
y=calerf(x,0)
endfunction
