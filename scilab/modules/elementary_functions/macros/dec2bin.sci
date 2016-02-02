// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2011 - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// =============================================================================
//
// Author : F.Belahcene
// DEC2BIN function
//
// Given x, a positive scalar/vector/matix of reals, this function returns
// a column vector of strings. Each string is the binary representation
// of the input argument components (i.e y(i) is the binary representation
// of x(i))
//
// -Inputs :
//    x : a  scalar/vector/matix of positives reals
//    n : an integer
// -Output :
//    y : a vector of strings (positives)
//
// =============================================================================

function y = dec2bin(x, n)
    rhs = argn(2);

    // check the number of input arguments
    if (rhs < 1 | rhs > 2) then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "dec2bin", 1, 2));
    end

    if rhs == 2 then
        y = dec2base(double(x), 2, n);
    else
        y = dec2base(double(x), 2);
    end
endfunction

