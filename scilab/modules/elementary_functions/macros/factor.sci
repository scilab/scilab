// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y=factor(x)

    // FACTOR function
    // Given a real x , this function returns a vector whose the components are the primes factors of the x decompositon.
    // Particular cases:
    // if x=0, then returns 0
    // if x=1, then return 1
    // Inputs :
    //  x : a real scalar (positive)
    // output :
    // y : a vector of primes numbers

    rhs = argn(2);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"factor", 1));
    end

    if prod(size(x,"*"))<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: An integer expected.\n"),"factor",1));
    elseif(type(x)<>1) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: An integer expected.\n"),"factor",1));
    elseif x<0 | x-floor(x)<>0
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"),"factor",1));
    elseif or(x==[0 1 2 3])
        y=x
    else
        xprimefact = primes(sqrt(x))
        y = []
        while x > 1
            xdiv = x ./ xprimefact
            xprimefact_ind = find(xdiv-floor(xdiv)==0)
            if xprimefact_ind <> []
                xprimefact = xprimefact(xprimefact_ind)
                y = [xprimefact y]
                x = x/prod(xprimefact)
            else
                y=[x y]
                break
            end
        end
    end
    y = gsort(y,"c","i")
endfunction
