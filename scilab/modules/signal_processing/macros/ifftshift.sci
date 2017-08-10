// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Allan Cornet
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
