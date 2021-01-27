// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2021 - Samuel GOUGEON - Le Mans Université
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [res1, res2] = %sp_det(A)
    [lhs, rhs] = argn(0);
    if length(A)==0 then
        [res1, res2] = (1,1);
        if lhs>1
            res1 = 0
        end
        return
    end
    if nnz(A)==0 then
        [res1, res2] = (0,0)
        return
    end

    wstatus = warning("query"); warning("off")  // to avoid the "is singular" message
    hand = umf_lufact(A);          // umfpack is used for complex sparse matrix
    [L,U,P,Q,r] = umf_luget(hand);
    umf_ludel(hand);
    warning(wstatus)

    dU = clean(diag(U), 0, %eps);
    r = clean(r, 0, %eps);
    if isreal(A) then
        p = r .* dU;
        tmp = sum(log10(full(abs(p))))
        res1 = int(tmp)
        if res1 < 0
            res1 = res1 - 1
        end
        res2 = 10^(tmp-res1) * prod(sign(p))
    else
        tmp = sum(log([r full(dU)]))
        phase = imag(tmp)
        e10 = real(tmp)/log(10) + log10(abs(cos(phase)))
        res1 = int(e10)
        res2 = 10^(real(tmp)/log(10)-res1) * exp(%i*phase)
        while abs(res2) >= 10
            res2 = res2 / 10
            res1 = res1 + 1
        end
        while abs(res2)<1 & abs(res2)<>0
            res2 = res2 * 10
            res1 = res1 - 1
        end
    end
    if res1 == -%inf
        [res1, res2] = (0,0)
    end
    if lhs == 1 then
        res1 = res2 * 10^res1
    end
endfunction
