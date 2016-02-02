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

function r=not_empty(A)
    // M2SCI function
    // Checks if no A dimension can be 0
    // Input: A = a M2SCI tlist
    // Output: r = boolean value (true if A can not be an empty matrix)

    n=size(A.dims)
    nb=0
    for k=1:n
        if A.dims(k)>0 | A.dims(k)==SupToOne | A.dims(k)==NotNull then
            nb=nb+1
        end
    end

    r=nb>0
endfunction
