// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [h]=buttmag(order,omegac,sample_vector)
//<h>=buttmag(order,omegac,sample_vector)
//Squared Magnitude response of a Butterworth filter
//omegac = cutoff frequency ; sample_vector = sample of frequencies
//  order  :Filter order
//  omegac :Cut-off frequency in Hertz
//  sample_vector :Vector of frequency where buttmag is evaluated
//  h      :Butterworth filter values at sample points
//
//!

// For ascendant compatibility before bug 4618 fix
// http://bugzilla.scilab.org/show_bug.cgi?id=4618
// In case the users calls buttmag with named arguments
if exists("sample","local")==1 then
  sample_vector = sample;
end

[n1,n2]=size(sample_vector);
un=ones(n1,n2);
h=un./(un+(sample_vector/omegac)**(2*order));
endfunction
