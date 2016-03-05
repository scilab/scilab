// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [n,m,x,y,nt,mt,xt,yt]=dcf(g,polf,polc,tol)
    //[n,m,x,y,nt,mt,xt,yt]=dcf(g,[polf,polc,[tol]]) returns eight
    //stable systems (n,m,x,y,nt,mt,xt,yt) for the doubly coprime factorization
    //
    //                 !xt  -yt! ! m  y !
    //                 !       !*!      ! = eye
    //                 !-nt  mt! ! n  x !
    // G must be stabilizable and detectable.
    // See copfac for a description of parameters.
    //!

    [lhs,rhs]=argn(0),
    n1=contr(g(2),g(3)),n2=contr(g(2)',g(4)'),
    select rhs,
    case 1 then
        polc=-ones(1,n1),polf=-ones(1,n2),tol=1000*%eps,
    case 2 then
        tol=polf,polc=-ones(1,n1),polf=-ones(1,n2),
    case 3 then tol=1000*%eps,
    end,
    [n,m,xt,yt]=copfac(g,polf,polc,tol),
    [nt,mt,x,y]=copfac(g',polc,polf,tol),
    nt=nt',mt=mt',x=x',y=y',
endfunction
