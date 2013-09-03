// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
