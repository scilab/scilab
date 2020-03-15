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

function a = %sp_r_sp(p, b)
    // a = p / b, such that p = a * b
    // p sparse, b sparse
    // not-square complex b not supported

    [rp, cp] = size(p)
    [rb, cb] = size(b)
    if cp <> cb then
        msg = _("%s: Arguments #%d and #%d: Same numbers of columns expected.\n")
        error(msprintf(msg, "%sp_r_sp", 1, 2))
    end
    if issquare(b)
        if det(b) <> 0 then
            a = p * inv(b)
            return
        end
    else
        p = p * b.'
        b = b * b.'   // Makes b square
    end

    [h,rk] = lufact(b.')
    if rk < min(rb,cb) then
        warning(msprintf(_("sparse / sparse: Deficient rank: rank = %d"),rk))
    end
    a = []
    for k = 1:rp
        a = [a ; sparse(lusolve(h,full(p(k,:)).').')]
    end
    ludel(h)
endfunction
