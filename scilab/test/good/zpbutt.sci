function [pols,gain]=zpbutt(n,omegac)
//<pols,gain>=zpbutt(n,omegac)
//Computes the poles of a Butterworth analog
//filter of order n and cutoff frequency omegac
//transfer function H(s) is calculated by
//     H(s) = gain/real(poly(pols,'s'))
//  n      :Filter order
//  omegac :Cut-off frequency in Hertz
//  pols   :Resulting poles of filter
//  gain   :Resulting gain of filter
//
//!
//Author F.D.
//Revised by C. Bunks Oct. 24, 1996  
// Copyright INRIA
angles=ones(1,n)*(%pi/2+%pi/(2*n))+(0:n-1)*%pi/n;
pols=omegac*exp(%i*angles);
gain=abs((-omegac)^n);
endfunction
