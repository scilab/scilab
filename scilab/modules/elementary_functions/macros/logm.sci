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

function x=logm(a)
    //   logm - log(A)
    //%SYNTAX
    //   X=logm(A)
    //%PARAMETERS
    //   A   : square hermitian or diagonalizable matrix
    //   X   : square matrix
    //%DESCRIPTION
    //computes X=logm(A), matrix log of A

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"logm", 1));
    end

    [m ,n] = size(a);

    if m <> n then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Square matrix expected.\n"),"logm",1));
    end

    flag = or(a<>a');
    if ~flag then
        //Hermitian matrix
        r = and(imag(a) == 0)
        [u, s] = schur(a);
        w = diag(s);
        zw = find(w == 0);
        if zw <> [] then
            w(zw) = %eps * ones(zw);
            w1 = log(w);
            w1(zw) = -%inf * ones(zw);
            warning(msprintf(gettext("%s: Log of a singular matrix.\n"),"logm"));
        else
            w1 = log(w);
        end
        x = u * diag(w1) * u';
        if r then
            if and(real(s) >= 0) then
                x = real(x);
            end
        end
    end
    if flag then
        //General matrix
        r = and(imag(a) == 0);
        a = a + 0 * %i;   //Set complex
        rmax = max(norm(a, 1), 1 / sqrt(%eps));
        [s, u, bs] = bdiag(a, rmax);
        if max(bs)>1 then
            error(msprintf(gettext("%s: Unable to diagonalize.\n"),"logm"));
            return
        end
        w = diag(s);
        zw = find(w == 0);
        if zw <> [] then
            w(zw) = %eps * ones(zw);
            w1 = log(w);
            w1(zw) = -%inf * ones(zw);
            warning(msprintf(gettext("%s: Log of a singular matrix.\n"),"logm"));
        else
            w1 = log(w);
        end
        x = (u * diag(w1)) * inv(u);
    end
endfunction
