// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [t] = acosh(z)
    //
    //  PURPOSE
    //     element wise hyperbolic arccosinus
    //
    //  METHOD
    //     based on the formula :
    //
    //     acosh(z) = sign(-imag(acos(z)) i acos(z)
    //
    //     but as in Scilab sign(0) = 0 (and this don't
    //     work in the formula which needs sign(0)=1)
    //     the following slower trick is used :
    //
    //     sign(-x) = 2*(0.5 - bool2s(x>0))
    //

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"acosh",1));
    end

    if type(z)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real or complex matrix expected.\n"),"acosh",1));
    end

    if isreal(z) then
        if min(z) < 1 then
            // result is complex
            u = acos(z)
            t = 2*(0.5 - bool2s(imag(u)>0)).*imult(u)
        else
            // result is real
            t = imag(acos(z));
            t(isnan(z)) = %nan;
        end
    else
        u = acos(z)
        t = 2*(0.5 - bool2s(imag(u)>0)).*imult(u)
    end
endfunction
