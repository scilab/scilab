function y=erfcx(x)
//erfcx  -  The scaled complementary error function.
//%Syntax
// y = erf(x).
//%Parameters
// x : real vector
// y : real vector (of same size)
//%Description
// erf computes the scaled complementary error function:
//
//      y = exp(x^2) * erfc(x) ~ (1/sqrt(pi)) * 1/x for large x.
//%See also 
// erfc erf calerf

// Copyright INRIA
y=calerf(x,2)
endfunction
