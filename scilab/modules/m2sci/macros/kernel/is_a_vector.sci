// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=is_a_vector(A)
    // M2SCI function
    // Checks if all dimensions of A but one are 1
    // Input: A = a M2SCI tlist
    // Output: r = boolean value (true if A is a vector)

    nbones=0
    n=size(A.dims)
    r=%F
    for k=1:n
        if A.dims(k)==1 then
            nbones=nbones+1
        elseif A.dims(k)==-1 then
            return
        end
    end
    r=nbones==n-1
endfunction
