function [y]=%asn(x,m)
//Calculates the elliptic integral:
//  y = integral from 0 to x of
//       [1/(((1-t*t)^(1/2))(1-m*t*t)^(1/2))]
//For vector arguments y is a vector
//  x :Upper limit of integral (x>0)
//  m :Parameter of integral (0<m<1)
//  y :Value of the integral
//
//!
//Author F.D.
// Copyright INRIA
m=real(m);
if m<0 then error('m must be positive');end 
if m>1 then error('m must be lower than 1');end 
y=delip(x,sqrt(m));
endfunction
