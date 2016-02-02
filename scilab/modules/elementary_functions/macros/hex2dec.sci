// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
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
//
// hex2dec function
//
// hex2dec(h) returns in vector d the numbers corresponding to the
// hexadecimal representation h.
//
// -Input :
//  str : a string (or a vector/matrix of strings)
// -Output :
//  y : a scalar/vector/matrix
//
// =============================================================================

function d = hex2dec(h)

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"hex2dec", 1));
    end

    if type(h)<> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"hex2dec",1));
    end

    d = base2dec(h, 16);

endfunction
