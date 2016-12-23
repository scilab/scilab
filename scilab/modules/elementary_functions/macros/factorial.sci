// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y=factorial(n)

    // This function returns the factorial n. If n is a vector, matrix or hypermatrix it returns the element wise factorial
    // Input : n, a scalar/vector/matrix/hypermat of positives integers.
    // Output : y, a scalar/vector/matrix/hypermat

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"factorial",1));
    end

    if (type(n) <> 1) | (~isempty(n) & (or((n-floor(n)<>0)) | or(n<0))) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Scalar/vector/matrix/hypermatrix of positive integers expected.\n"),"factorial",1));
    elseif isempty(n)
        y=n
        return
    else
        n(n==0)=1
        ntemp=cumprod(1:max(n))
        y=matrix(ntemp(n(:)),size(n))
    end

endfunction
