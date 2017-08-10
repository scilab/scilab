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


function [x,err]=diophant(p1p2,b)
    //solves diophantine equation p1*x1+p2*x2=b
    //with  p1p2 a polynomial vector [p1 p2]
    //b polynomial
    //x polynomial vector [x1;x2]
    //if the equation is uncompatible err=||p1*x1+p2*x2-b||/||b||
    //else err=0
    //!
    p1=p1p2(1);p2=p1p2(2)
    [x,u]=bezout(p1,p2)
    p1=u(2,2);p2=u(1,2)//
    if degree(x)==0 then
        x=b*u(:,1)
        err=0
    else
        [r,q]=pdiv(b,x)
        err=norm(coeff(b-x*q),2)/norm(coeff(b),2)
        x=q*u(:,1)
    end
endfunction
