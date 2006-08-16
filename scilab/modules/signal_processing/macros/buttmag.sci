function [h]=buttmag(order,omegac,sample)
//<h>=buttmag(order,omegac,sample)
//Squared Magnitude response of a Butterworth filter
//omegac = cutoff frequency ; sample = sample of frequencies
//  order  :Filter order
//  omegac :Cut-off frequency in Hertz
//  sample :Vector of frequency where buttmag is evaluated
//  h      :Butterworth filter values at sample points
//
//!
//Author F.D.
// Copyright INRIA

   [n1,n2]=size(sample);
   un=ones(n1,n2);
   h=un./(un+(sample/omegac)**(2*order));
endfunction
