// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y=factorial(n)

    // This function returns the factorial n. If n is a vector, matrix or hypermatrix it returns the element wise factorial
    // Input : n, a scalar/vector/matrix/hypermat of positives integers.
    // Output : y, a scalar/vector/matrix/hypermat

    rhs = argn(2);

    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"factorial",1));
    end

    if (type(n) <> 1 & (typeof(n)<>"hypermat" | type(n.entries)<>1)) | (~isempty(n) & (or((n-floor(n)<>0)) | or(n<0))) then
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
