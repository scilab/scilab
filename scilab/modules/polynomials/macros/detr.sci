// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
