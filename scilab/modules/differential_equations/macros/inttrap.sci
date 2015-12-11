// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
