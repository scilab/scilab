// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - G. Le Vey
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [la, sig, lb] = levin(n, Cov)
    // [la, sig, lb] = levin(n, Cov)
    // macro which solves recursively on n
    // the following Toeplitz system (normal equations)
    //
    //
    //		|R1   R2   . . . Rn  |
    //		|R0   R1   . . . Rn-1|
    //		|R-1  R0   . . . Rn-2|
    //		| .    .   . . .  .  |
    // |I -A1 . -An|| .    .   . . .  .  |
    //		| .    .   . . .  .  |
    //		| .    .   . . .  .  |
    //		|R2-n R3-n . . . R1  |
    //		|R1-n R2-n . . . R0  |
    //
    //  where {Rk;k=1,nlag} is the sequence of nlag empirical covariances
    //
    //  n   : maximum order of the filter
    //  Cov : matrix containing the Rk (d*d matrices for a
    //      : d-dimensional process). It must be given the
    //      : following way:
    //
    //		|  R0  |
    //		|  R1  |
    //		|  R2  |
    //		|  .   |
    //		|  .   |
    //		| Rnlag|
    //
    //  la  : list-type variable, giving the successively calculated
    //      : polynomials (degree 1 to degree n), with coefficients Ak
    //  sig : list-type variable, giving the successive
    //      : mean-square errors.
    //!

    [lhs, rhs] = argn(0);
    if rhs <> 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "levin", 2));
    end
    [l, d] = size(Cov);
    if d > l then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A tall matrix expected.\n"), "levin", 2));
    end
    //
    //   Initializations
    //
    a = eye(d, d);
    b = a;
    z = poly(0, "z");
    la = list();
    lb = list();
    sig = list();
    p = n+1;
    cv = Cov;
    for j=1:p
        cv = [Cov(j*d+1:(j+1)*d, :)'; cv];
    end
    for j=0:n-1
        //
        //   Levinson algorithm
        //
        r1 = flipdim(cv((p+1)*d+1:(p+2+j)*d, :), 1, d);
        r2 = flipdim(cv(p*d+1:(p+1+j)*d, :),     1, d);
        r3 = flipdim(cv((p-1-j)*d+1:p*d, :),     1, d);
        r4 = flipdim(cv((p-j)*d+1:(p+1)*d, :),   1, d);
        c1 = coeff(a);
        c2 = coeff(b);
        sig1 = c1*r4;
        gam1 = c2*r2;
        k1 = (c1*r1)*inv(gam1);
        k2 = (c2*r3)*inv(sig1);
        a1 = a-k1*z*b;
        b = -k2*a+z*b;
        a = a1;
        la(j+1) = a;
        lb(j+1) = b;
        sig(j+1) = sig1;
    end
endfunction
