// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

function x = hilbert(xr)
    // Marple, S.L., "Computing the discrete-time analytic signal via FFT,"
    // IEEE Transactions on Signal Processing, Vol. 47, No.9 (September
    // 1999), pp.2600-2603
    // http://ieeexplore.ieee.org/iel5/78/16975/00782222.pdf?arnumber=782222

    if  type(xr)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"hilbert",1))
    end

    n=size(xr,"*");

    if n==0 then x=[],return,end
    if ~isreal(xr,0) then
        error(msprintf(gettext("%s: Input argument #%d must be real.\n"),"hilbert",1));
    end

    no2 = int(n/2);

    x = fft(real(xr),-1);

    h  = zeros(xr);

    if ((2*no2) == n) then  // n is even
        h([1,no2+1]) = 1;
        h(2:no2) = 2;
    else // n is odd
        h(1) = 1;
        h(2:(n+1)/2) = 2;
    end

    x = fft(x.*h,1);

endfunction

