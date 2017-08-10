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

function [a,b,sig]=reglin(x,y,dflag)
    // Solves a linear regression
    // y=a(p,q)*x+b(p,1) + epsilon
    // x : matrix (q,n) and y matrix (p,n)
    // sig : noise standard deviation
    // dflag is optional if 1 a display of the result is done
    //!

    [lhs,rhs]=argn(0);
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"reglin",2,3))
    end
    if rhs <=2;dflag=0;end
    [n1,n2]=size(x)
    [p1,p2]=size(y)
    if n2<>p2 then
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),"reglin",1,2));
    end;
    if or(isnan(x)) | or(isnan(y)) then
        error(msprintf(_("%s: NaNs detected, please use %s() instead.\n"), "reglin", "nanreglin"))
    end

    xmoy=sum(x,2)/n2
    ymoy=sum(y,2)/n2
    // We use armax for appropriate orders which will perform
    // nothing but a least square
    // We could directly call pinv or \
    [arc,la,lb,sig]=armax(0,0,y-ymoy*ones(1,n2),x-xmoy*ones(1,n2),0,dflag);
    if typeof(la)=="list" then a=lb(1);else a=lb;end
    b=ymoy-a*xmoy;
endfunction
