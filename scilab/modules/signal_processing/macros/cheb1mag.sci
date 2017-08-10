// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function [h2]=cheb1mag(n,omegac,epsilon,sampleFreq)
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


    [n1,n2]=size(sampleFreq);
    un=ones(n1,n2);
    Tn=chepol(n,"x");  //n-th Chebyshev polynomial
    fr=freq(Tn,1,sampleFreq/omegac);   //fr=Tn(sample/omegac)
    h2=un./(un+epsilon*epsilon*fr.*fr)   //magnitude
endfunction
