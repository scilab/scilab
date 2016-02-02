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


function [d]=detr(h)
    //[d]=detr(h)  computes the determinant of a polynomial or
    //rational function matrix h using Leverrier's method
    //!
    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "detr", 1));
    end
    
    tof=typeof(h)
    if or(tof==["polynomial","constant", "rational"]) then
        [m,n]=size(h);
        if m<>n then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),"detr",1))
        end
        f=eye(n,n);
        for k=1:n-1,
            b=h*f,
            d=-sum(diag(b))/k
            f=b+eye(n,n)*d,
        end
        d=-sum(diag(h*f))/n;
        if 2*int(n/2)<>n then d=-d;end
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A floating point number or polynomial or rational fraction array expected.\n"),"detr",1))
    end
endfunction
