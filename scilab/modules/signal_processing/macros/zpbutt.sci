// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// Copyright (C) INRIA - 1996 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [pols,gain]=zpbutt(n,omegac)
    //<pols,gain>=zpbutt(n,omegac)
    //Computes the poles of a Butterworth analog
    //filter of order n and cutoff frequency omegac
    //transfer function H(s) is calculated by
    //     H(s) = gain/real(poly(pols,'s'))
    //  n      :Filter order
    //  omegac :Cut-off frequency in Hertz rd/s
    //  pols   :Resulting poles of filter
    //  gain   :Resulting gain of filter
    //
    //!
    angles=ones(1,n)*(%pi/2+%pi/(2*n))+(0:n-1)*%pi/n;
    pols=omegac*exp(%i*angles);
    gain=abs((-omegac)^n);
endfunction
