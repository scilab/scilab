
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function A = companion(p)
    // Companion matrix.
    // A=companion(p) is a companion matrix with
    // characteristic polynomial equal to (or proportional to)
    // p. If p is a row or column vector of polynomials, the A matrix is block
    // diagonal and block number i has characteristic polynomial
    // equal to p(i).

    if type(p) ~= 2
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Polynomial expected."),"companion",1));
    end
    // Tranform the row or column vector of poly into a column vector of polynomials
    p=p(:);
    // Transpose the vector from column in to row vector,
    // so that the "for" loop can work properly for each poly.
    // Caution : ".'", NOT "'"
    p=p.';
    A=[];
    //for pp=p;
    polynumber = length(p);
    for polyindex=1:polynumber;
        pp=p(polyindex);
        c=coeff(pp);
        // Reverse the order of the coefficients, so that the coefficient associated with s^n
        // comes first.
        c=c($:-1:1);
        n = length(c);
        if n <= 1
            B=[];
        elseif n == 2
            B=-c(2)/c(1);
        else
            // Caution : ".'", NOT "'"
            c=c(:).';
            B=diag(ones(1,n-2),-1);
            B(1,:) = -c(2:n)/c(1);
        end
        A=sysdiag(A,B);
    end
endfunction
