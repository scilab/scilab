// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// NO LONGER CALLED (hm of booleans, integers, decimal or complex number, polynomials, strings)
// => TO BE DELETED

function r=%hm_n_hm(a,b)
    r=and(a.dims==b.dims)
    if r then
        r=matrix(a.entries<>b.entries, a.dims)
    else
        r=%t
    end
endfunction
