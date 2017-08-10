// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x]=filt_sinc(n,fl)

    //x=sinc(n,fl)
    //Calculate n samples of the function sin(2*pi*fl*t)/(pi*t)
    //for t=-n/2:n/2 (i.e. centered around the origin).
    //  n  :Number of samples
    //  fl :Cut-off freq. of assoc. low-pass filter in Hertz
    //  x  :Samples of the sinc function

    no2  = (n-1)/2;
    ino2 = int(no2);
    wl   = fl*2*%pi;
    xn   = sin(wl*(-no2:no2));
    xd   = %pi*(-no2:no2);
    if ino2==no2 then
        xn(no2+1) = 2*fl;
        xd(no2+1) = 1;
    end
    x=xn./xd;

endfunction
