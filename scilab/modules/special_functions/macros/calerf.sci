// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Steven G. Johnson <stevenj@alum.mit.edu>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
