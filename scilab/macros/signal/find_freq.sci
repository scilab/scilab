function [m]=find_freq(epsilon,A,n)
//Search for m such that n=K(1-m1)K(m)/(K(m1)K(1-m))
//with m1=(epsilon*epsilon)/(A*A-1);
//If  m = omegar^2/omegac^2,the parameters
//epsilon,A,omegac,omegar and n are then
//compatible for defining a prototype elliptic filter.
//  epsilon :Passband ripple
//  A       :Stopband attenuation
//  n       :filter order
//  m       :Frequency needed for construction of
//          :elliptic filter
//
//!
//Author F.D.
// Copyright INRIA

   m1=(epsilon*epsilon)/(A*A-1);
   chi1=%k(1-m1)/%k(m1);
   m=findm(chi1/n);
 
endfunction
