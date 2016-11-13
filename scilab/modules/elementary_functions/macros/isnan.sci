// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = isnan(x)
    rhs = argn(2);
    if rhs <> 1 then
        msg = gettext("%s: Wrong number of input argument(s): %d expected.\n")
        error(msprintf(msg, "isnan", 1));
    end
    if x == [] then
        r = [];
    else
        if or(type(x)==[1 5])   // for dense and sparse decimal encodings
            if isreal(x)
                r = (x ~= x);
            else
                rp = real(x)
                ip = imag(x)
                r = (x~=x | (abs(real(x))==%inf & abs(imag(x))==%inf) )
            end
        else
            r = ~(x == x);
            // the case of polynomials will be better managed apart,
            // after merging this first fix for complex numbers
        end
    end
endfunction
