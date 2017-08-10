// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function x = sqrtm(a)
    //   sqrtm - computes the matrix square root.
    //%SYNTAX
    //   x=sqrtm(a)
    //%PARAMETERS
    //   a   : square hermitian or diagonalizable matrix
    //   x   : square matrix
    //!

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"sqrtm", 1));
    end
    if type(a) <> 1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A real or complex matrix expected.\n"),"sqrtm", 1));
    end
    if ~issquare(a) then
        error(msprintf(_("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"sqrtm", 1));
    end

    x = %s_pow(a, 0.5);

endfunction
