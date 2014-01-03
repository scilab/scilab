// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
