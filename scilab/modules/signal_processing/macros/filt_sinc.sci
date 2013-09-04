// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
