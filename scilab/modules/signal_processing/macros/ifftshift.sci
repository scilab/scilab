// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Allan Cornet
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function y = ifftshift(x)

    if argn(2) < 1 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "ifftshift", 1));
    end

    numDims = ndims(x);
    idx = list(1, numDims);

    for k = 1:numDims
        m = size(x, k);
        p = floor(m/2);
        idx(k) = [p+1:m 1:p];
    end

    y = x(idx(:));

endfunction
