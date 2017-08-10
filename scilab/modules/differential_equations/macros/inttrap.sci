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

function v = inttrap(x,y)
    //inttrap  Trapezoidal numerical integration.
    //v = inttrap(x,y) computes the integral of y with respect to x using
    //trapezoidal integration.  x and y must be vectors of the same dimension
    //v = inttrap(y) computes the trapezoidal integral of y assuming unit
    //spacing between the data points.

    [lhs,rhs]=argn(0)
    if rhs<2 then
        y=x;
        v=sum(y(1:$-1) + y(2:$))/2;
    else
        if size(x,"*")<>size(y,"*") then
            error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"inttrap"));
        end
        x=x(:);y=y(:);
        v=(x(2:$)-x(1:$-1))'*(y(1:$-1) + y(2:$))/2;
    end
endfunction
