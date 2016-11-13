// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function t=asinhm(x)
    // Matrix wise Hyperbolic sine inverse of x
    // Entries of x must be in ]-1,i[
    // Entries of t are in    ]-inf,inf[ x ]-pi/2,pi/2[
    //                             ]-inf, 0 ] x [-pi/2]
    //                      and    [ 0  ,inf[ x [ pi/2]
    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"asinhm",1));
    end

    if type(x)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"asinhm",1));
    end

    [m,n]=size(x);

    if m<>n then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"asinhm",1));
    end

    t=logm(x+sqrtm(x*x+eye()));

endfunction
