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

function y = primes(x)

    // PRIMES function
    // Given a real x, this function returns a vector whose the components are all the primes numbers between 1 and x (for x>0), else returns an empty matrix
    // Inputs :
    //  x : a real scalar
    // output :
    // y : a vector of reals (positives)

    [lhs, rhs] = argn(0);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "primes", 1));
    end

    if type(x) <> 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"),"primes"));
    elseif prod(size(x,"*")) <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A real expected.\n"),"primes"));
    elseif x < 1
        y = [];
    else
        y = 1:fix(x);
        ylength = size(y,"*")
        i = 2
        while (i <= sqrt(x))
            if y(i) <> 0
                y(2*y(i):y(i):ylength) = 0
            end
            i = i+1
        end
        y(y<=1)=[];
    end

endfunction
