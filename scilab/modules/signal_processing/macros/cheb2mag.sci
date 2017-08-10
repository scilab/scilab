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

