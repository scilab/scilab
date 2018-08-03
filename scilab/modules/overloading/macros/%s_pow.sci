// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function x=%s_pow(a,p)
    //   g_pow - A^p special cases
    //%SYNTAX
    //   X=g_pow(A,p)
    //%PARAMETERS
    //   A   : constant or square hermitian or diagonalizable matrix
    //   X   : square matrix
    //   p   : square matrix or scalar
    //%DESCRIPTION
    //This function is called by the operation ^ to compute A^p in special cases
    // - A scalar and p square matrix
    // - A square matrix p is not an integer
    //!
    [m,n] = size(a)
    [mp,np] = size(p)
    if (m*n == 1) && (mp <> np) then
        error(msprintf(_("%s: Wrong size for input argument #%d: Square matrix expected.\n"), "%s_pow", 2));
    end
    if (mp*np == 1) && (m <> n) then
        error(msprintf(_("%s: Wrong size for input argument #%d: Square matrix expected.\n"), "%s_pow", 1));
    end
    if (m*n == 1) && (mp==np) then  //a^P
        flag = or(p <> p')
        r = and(imag(p) == 0) && (imag(a) == 0) && ( a>= 0)
        if ~flag then
            //Hermitian matrix
            [u, s] = schur(p);
            w=a .^ diag(s);
            x=u * diag(a .^ diag(s)) * u';
            if r then
                x = real(x)
            end
        else
            x = expm(log(a) * p);
        end
        if r then x = real(x), end
    elseif (m == n) && (mp*np == 1) then  //A^p  p non integer
        flag = or(a <> a')
        if ~flag then
            //Hermitian matrix
            r = and(imag(a) == 0)
            [u, s] = schur(a);
            x = u * diag(diag(s) .^ p) * u';
            if r then
                if (s >= 0) && (imag(p) == 0) then
                    x = real(x)
                end
            end
        else
            //General matrix
            r = and(imag(a) == 0)
            [s, u, bs] = bdiag(a + 0*%i);
            if (max(bs) > 1) then
                error(msprintf(_("%s: Unable to diagonalize.\n"),"%s_pow"));
            end
            x = u * diag(diag(s) .^ p) * inv(u);
        end
        if (int(p) == p) && (real(p) == p) && r then
            x = real(x);
        end
    else
        error(msprintf(_("Not implemented in scilab...\n")));
    end
endfunction
