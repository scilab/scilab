// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// =============================================================================
//
// oct2dec function
//
// oct2dec(o) returns in vector d the numbers corresponding to the
// octal representation of o.
//
// -Input :
//  str : a string (or a vector/matrix of strings)
// -Output :
//  y : a scalar/vector/matrix
//
// =============================================================================

function d = oct2dec(o)

    [lhs, rhs] = argn(0);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"oct2dec", 1));
    end

    if type(o) <>  10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "oct2dec", 1));
    end

    d = base2dec(o, 8);

endfunction
