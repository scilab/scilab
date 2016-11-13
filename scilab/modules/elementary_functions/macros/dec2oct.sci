// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Pierre MARECHAL
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
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
// dec2oct function
// =============================================================================

function y = dec2oct(x)

    rhs = argn(2);

    // check the number of input arguments
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "dec2oct", 1));
    end

    if or(type(x) <> 8) & (or(type(x) <> 1) | or(x < 0) ) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Scalar/vector/matrix of positive integers expected.\n"), "dec2oct", 1));
    end

    y = dec2base(double(x), 8);

endfunction
