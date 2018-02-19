// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) Scilab Enterprises - 2011 - Calixte DENIZET
// Copyright (C) 2017 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function a = toeplitz(c, r)
    // a = toeplitz(c,r)  returns the Toepliz matrix whose first row
    // is r and first column is c .( r(1) = c(1) is assumed).

    // Checking input arguments
    // ------------------------
    rhs = argn(2);
    if rhs < 1 then
        msg = _("%s: Wrong number of input argument(s): %d to %d expected.\n");
        error(msprintf(msg, "toeplitz", 1, 2));
    end
    if rhs == 1 then
        r = c;
    end
    
    if isempty(c) | isempty(r) then
        a = []
        return
    end
    // Checking types compatibility & upper left common corner:
    r = matrix(r, 1, -1);
    c = matrix(c, 1, -1);
    tmp = [c(1,1) r(1,1)];
    if tmp(1,1) <> tmp(1,2) then
        msg = _("%s: Wrong values for input arguments #%d and #%d: c(1) must be equal to r(1).\n");
        error(msprintf(msg, "toeplitz", 1, 2));
    end

    // Processing
    // ----------
    nr = size(r, "*");
    nc = size(c, "*");
    index = ones(1, nr) .*. ((nc - 1):-1:0)' + (1:nr) .*. ones(nc, 1);
    b = [c(1, $:-1:2) r];
    a = matrix(b(1,index(:)'), nc, nr);
endfunction
