function [v]=ell1mag(eps,m1,z)
//Function used for squared magnitude of an elliptic filter
//Usually m1=eps*eps/(a*a-1);
//  eps     :Passband ripple=1/(1+eps**2)
//  m1      :Stopband ripple=1/(1+(eps**2)/m1)
//  z       :Sample vector of values in the complex plane
//  v       :Elliptic filter values at sample points
//
//!
// Copyright INRIA
s=%sn(z,m1);un=ones(z);
v=real(un./(un+eps*eps*s.*s))

 
endfunction
