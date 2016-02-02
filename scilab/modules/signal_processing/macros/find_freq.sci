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

function [m]=find_freq(epsilon,A,n)
    //Search for m such that n=K(1-m1)K(m)/(K(m1)K(1-m))
    //with m1=(epsilon*epsilon)/(A*A-1);
    //If  m = omegar^2/omegac^2,the parameters
    //epsilon,A,omegac,omegar and n are then
    //compatible for defining a prototype elliptic filter.
    //  epsilon :Passband ripple
    //  A       :Stopband attenuation
    //  n       :filter order
    //  m       :Frequency needed for construction of
    //          :elliptic filter
    //
    //!

    m1=(epsilon*epsilon)/(A*A-1);
    chi1=%k(1-m1)/%k(m1);
    m=findm(chi1/n);

endfunction
