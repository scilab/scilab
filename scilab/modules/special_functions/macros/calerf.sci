// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Steven G. Johnson <stevenj@alum.mit.edu>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y=calerf(x,flag)

    //calerf  -  computes error functions
    //%Syntax
    // y = calerf(x,flag).
    //%Parameters
    // x : real (or complex) vector
    // flag : integer indicator (0, 1, or 2)
    // y : real (or complex) vector (of same size)
    //%Description
    // calerf computes the error functions erf, erfc, or erfcx,
    // depending on the value of the flag parameter.  (It is
    // included for backward compatibility with older Scilab
    // versions; there is no longer any reason to call this directly.)
    //%See also
    // erf erfc erfcx erfi dawson
    //!

    rhs = argn(2);

    // check the number of input arguments
    if rhs<>2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"calerf",2));
    end

    if flag==0 then
        y=erf(x);
    elseif flag==1 then
        y=erfc(x);
    elseif flag==2 then
        y=erfcx(x);
    else
        error(msprintf(gettext("%s: Wrong value for argument #%d: 0, 1 or 2 expected.\n"),"calerf",2));
    end

endfunction
