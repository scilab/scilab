// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function [h2]=cheb2mag(n,omegar,A,samplefreq)
    //<h2>=cheb2mag(n,omegar,A,samplefreq)
    //Square magnitude response of a type 1 Chebyshev filter
    //omegar = stopband edge
    //samplefreq = vector of frequencies where the square magnitude
    //h2 is desired.
    //  n       :Filter order
    //  omegar  :Cut-off frequency
    //  A       :Attenuation in stop band
    //  samplefreq  :Vector of frequency where cheb2mag is evaluated
    //  h2      :Chebyshev II filter values at sample points
    //
    //!


    [n1,n2]=size(samplefreq);
    un=ones(n1,n2);
    Tn=chepol(n,"x");             //n-th Chebyshev polynomial
    frd=freq(Tn,1,omegar*un./samplefreq);   //frd=Tn(omegar/samplefreq)
    h2=un./(un+(A*A-1)*un./real(frd.*frd))
endfunction

