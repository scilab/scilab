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

function v = intsplin(x,s)
    //splin  numerical integration.
    //v = intsplin(x,s) computes the integral of y with respect to x using
    //splin interpolation and integration.
    //x and y must be vectors of the same dimension
    //
    //v = intsplin(s) computes the integral of y assuming unit
    //spacing between the data points.

    [lhs,rhs]=argn(0)
    if rhs<2 then
        s=x;
        s=s(:);
        d=splin((1:size(s,"*"))',s);
        v=sum((d(1:$-1)-d(2:$))/12 + (s(1:$-1)+s(2:$))/2);
    else
        if size(x,"*")<>size(s,"*") then
            error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"intsplin"));
        end
    end
    x=x(:);s=s(:);
    d=splin(x,s);
    h=x(2:$)-x(1:$-1);
    v=sum((h.*(d(1:$-1)-d(2:$))/12 + (s(1:$-1)+s(2:$))/2).*h);
endfunction
