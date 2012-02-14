// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) Scilab Enterprises - 2011 - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function a = toeplitz(c, r)
    //a=toeplitz(c,r)  returns the Toepliz matrix whose first row
    //is r and first column is c .( r(1) = c(1) is assumed).
    //
    //r and  c can be constant, polynomial or character string matrices.
    //!

    [lhs, rhs] = argn(0);
    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "toeplitz", 1));
    end

    if rhs == 1 then
        r = c(:);
    end

    nr = size(r, '*');
    r = matrix(r, 1, nr);
    nc = size(c, '*');
    c = matrix(c, 1, nc);

    if nr * nc == 0 then
        a = [];
        return;
    end

    if r(1) <> c(1) then
        error(msprintf(gettext("%s: Wrong values for input arguments #%d and #%d: c(1) must be equal to r(1).\n"), "toeplitz", 1, 2));
    end

    index = ones(1, nr) .*. ((nc - 1):-1:0)' + (1:nr) .*. ones(nc, 1);
    b = [c($:-1:2) r];
    a = matrix(b(index), nc, nr);
endfunction
