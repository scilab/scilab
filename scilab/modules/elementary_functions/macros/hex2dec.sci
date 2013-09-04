// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
