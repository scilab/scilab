// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = %p_part(txt, s)
    // Allows calls such as: part(txt, [ 2 $ 3:4 $-1])
    //  with un-ranging implicit size $ standing for length(txt)
    // Calls with ranging $ (ex: 3:$ ) are implemented with %ip_part()
    //
    // txt may be a matrix of texts with different lengths
    //  => $ might have not the same value for all elements of txt.
    if varn(s)~="$" then
        msg = _("%s: $ is the only implicit variable supported in selected indices")
        error(msprintf(msg,"part"))
    end
    L = length(txt)
    U = unique(L)
    r = emptystr(txt)
    for u = U(:)'
        j = 1:u
        k = find(L==u)
        r(k) = part(txt(k), horner(s(:)',u))
    end
endfunction
