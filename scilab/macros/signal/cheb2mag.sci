function [h2]=cheb2mag(n,omegar,A,sample)
//<h2>=cheb2mag(n,omegar,A,sample)
//Square magnitude response of a type 1 Chebyshev filter
//omegar = stopband edge
//sample = vector of frequencies where the square magnitude
//h2 is desired.
//  n       :Filter order
//  omegar  :Cut-off frequency
//  A       :Attenuation in stop band
//  sample  :Vector of frequency where cheb2mag is evaluated
//  h2      :Chebyshev II filter values at sample points
//
//!
//Author F.D.
// Copyright INRIA

   [n1,n2]=size(sample);
   un=ones(n1,n2);
   Tn=chepol(n,'x');             //n-th Chebyshev polynomial
   frd=freq(Tn,1,omegar*un./sample);   //frd=Tn(omegar/sample)
   h2=un./(un+(A*A-1)*un./real(frd.*frd))
endfunction
