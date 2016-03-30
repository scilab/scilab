
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

function [u,c]=givens(x,y)
    //Syntax : u=givens(xy)
    //         u=givens(x,y)
    //
    // xy = [x;y], u=givens(xy)
    // returns a 2*2 matrix u such that u*xy=[r;0].
    // c is equal to u*xy
    // givens(x,y)=givens([x;y])
    //
    //!
    [lhs,rhs]=argn(0);
    if rhs==2 then x=[x;y];end
    if or(size(x)<>[2 1]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A column vector expected.\n"),"givens",1));
    end
    if x(2)<>0 then
        r = norm(x);
        u = [x'; -x(2) x(1)]/r;
        c = [r; 0];
    else
        u=eye(2,2)
        c=x
    end
endfunction
