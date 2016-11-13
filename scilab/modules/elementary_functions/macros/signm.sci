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

function x = signm(a)
    //   signm - computes the matrix sign function.
    //%SYNTAX
    //   x=signm(a)
    //%PARAMETERS
    //   a   : square hermitian matrix
    //   x   : square hermitian matrix
    //%DESCRIPTION
    //   This macro is called by the function sign to compute square matrix
    //   sign function.

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"signm", 1));
    end

    [m, n] = size(a);

    if m <> n then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"signm",1));
    end

    flag = or(a <> a');

    if flag then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Non-hermitian matrix.\n"),"signm",1));
    end

    [u, s] = schur(a);
    x = u'*diag(sign(real(diag(s)))) * u;
endfunction
