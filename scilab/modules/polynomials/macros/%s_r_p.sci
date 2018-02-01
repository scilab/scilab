// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function f = %s_r_p(m, p)
    // f=M/p  M:scalar matrix p=polynomial

    if m==[] then
        f = []
        return
    end
    sp = size(p)
    if prod(sp) <> 1 then
        f = m * invr(p),
    else
        // p scalar
        m = m + poly(0,varn(p),"coeff")
        sm = size(m)
        // scalar / (eye()*%z  or eye() / p
        if prod(sm)==1 & sp(1)==-1  | sm(1)==-1 then
            f = rlist(m, p*eye(), [])
        else
            f = rlist(m, p*ones(m), [])
            f = simp(f)
        end
    end
endfunction
