function [h2]=cheb1mag(n,omegac,epsilon,sample)
//<h2>=cheb1mag(n,omegac,epsilon,sample)
//Square magnitude response of a type 1 Chebyshev filter
//omegac=passband edge
//epsilon such that 1/(1+epsilon**2)=passband ripple
//sample vector of frequencies where the square magnitude
//is desired.
//  n       :Filter order
//  omegac  :Cut-off frequency
//  epsilon :Ripple in pass band
//  sample  :Vector of frequency where cheb1mag is evaluated
//  h2      :Chebyshev I filter values at sample points
//
//!
//Author F.D.
// Copyright INRIA

   [n1,n2]=size(sample);
   un=ones(n1,n2);
   Tn=chepol(n,'x');  //n-th Chebyshev polynomial
   fr=freq(Tn,1,sample/omegac);   //fr=Tn(sample/omegac)
   h2=un./(un+epsilon*epsilon*fr.*fr)   //magnitude
endfunction
