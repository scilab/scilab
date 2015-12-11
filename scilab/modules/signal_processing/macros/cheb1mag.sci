// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
