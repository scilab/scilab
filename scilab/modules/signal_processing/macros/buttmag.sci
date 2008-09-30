// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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

   [n1,n2]=size(sample);
   un=ones(n1,n2);
   h=un./(un+(sample/omegac)**(2*order));
endfunction
