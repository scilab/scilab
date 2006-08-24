function t=asinhm(x)
// Matrix wise Hyperbolic sine inverse of x
// Entries of x must be in ]-1,i[
// Entries of t are in    ]-inf,inf[ x ]-pi/2,pi/2[
//                             ]-inf, 0 ] x [-pi/2]
//                      and    [ 0  ,inf[ x [ pi/2]
//!
// Copyright INRIA
  if type(x)<>1 then error(53,1),end
  [m,n]=size(x)
  if m<>n then 
    error(20)
  else 
    t=logm(x+sqrtm(x*x+eye()))
  end
endfunction
