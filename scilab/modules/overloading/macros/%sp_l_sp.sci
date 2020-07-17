// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function b = %sp_l_sp(a, p)
    // a sparse, p sparse, b sparse
    // b = a \ p   such that p = a * b

    [ma, na] = size(a)
    [mp, np] = size(p)
    if ma <> mp then
        msg = _("%s: Arguments #%d and #%d: Same numbers of rows expected.\n")
        error(msprintf(msg, "%sp_l_sp", 1, 2))
    end
    if issquare(a)
        if det(a) <> 0 then
            b = inv(a) * p
            return
        end
    else
        p = a.' * p
        a = a.' * a
    end

    [h,rk] = lufact(a)
    if rk < min(ma,na) then
        warning(msprintf(_("sparse \ sparse: Deficient rank: rank = %d"),rk))
    end
    b = []
    for k = 1:np
        b = [b, sparse(lusolve(h,full(p(:,k))))]
    end
    ludel(h)
endfunction
