function t=asinh(x)
// Element wise Hyperbolic sine inverse of x
// Entries of x must be in ]-1,i[
// Entries of t are in    ]-inf,inf[ x ]-pi/2,pi/2[
//                             ]-inf, 0 ] x [-pi/2]
//                      and    [ 0  ,inf[ x [ pi/2]
//!
// Copyright INRIA
if type(x)<>1 then error(53),end
t=log(x+sqrt(x.*x+ones(x)))




