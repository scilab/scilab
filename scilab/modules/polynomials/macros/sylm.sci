// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function s = sylm(a, b)
    //[s]=sylm(a,b) gives the Sylvester matrix associated to polynomials
    //a and b, i.e. the matrix s such that:
    //  coeff( a*x + b*y )' = s * [coeff(x)';coeff(y)']
    //dimension of s is equal to degree(a)+degree(b)
    //If a and b are coprime polynomials
    //(rank(sylm(a,b))=degree(a)+degree(b)) the instructions
    //  u = sylm(a,b) \ eye(na+nb,1)
    //  x = poly(u(1:nb),'z','coeff')
    //  y = poly(u(nb+1:na+nb),'z','coeff')
    //compute Bezout factors x et y of minimal degree de degre minimal
    //such that a*x+b*y=1
    // https://en.wikipedia.org/wiki/Sylvester_matrix
    //!
    na = max(0,degree(a)); ca = coeff(a).';
    nb = max(0,degree(b)); cb = coeff(b).';

    // Special cases
    if na==0 & nb==0 then
        s = []
        return
    end

    if degree(a)==0 & nb<>0 | degree(b)==0 & na<>0 then
        n = max(na,nb)
        s = eye(n,n)
        if na==0
            s = s*ca
        else
            s = s*cb
        end
        return
    end

    // General case
    s = zeros(na+nb, na+nb);
    for i = 1:nb,
        s(i:na+i,i) = ca
    end
    for i = 1:na,
        s(i:nb+i,nb+i) = cb
    end
endfunction
