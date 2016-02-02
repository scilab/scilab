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


function [s]=sylm(a,b)
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
    //!
    na=degree(a);a=coeff(a)';
    nb=degree(b);b=coeff(b)';
    s(na+nb,na+nb)=0;
    for i=1:nb,s(i:na+i,i)=a,end
    for i=1:na,s(i:nb+i,nb+i)=b,end
endfunction
