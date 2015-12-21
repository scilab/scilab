// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013, 2015 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r = %ip_part(txt, s)
    // Allows calls such as: part(txt, $:-1:1)  // (== strrev() )
    // Calls with unranging $ are not supported => define %p_part()
    // txt may be a matrix of texts with different lengths
    //  => $ might not have the same value for all of them

    //checking s is useless: a) it is necessarily an IP since %ip_... is called.
    //  b) it can be of any variable name, not necessarilly "$":
    //  part("abcde", %z:-1:1) works as well

    L = length(txt)
    r = emptystr(txt)
    if length(L)>100 then
        U = unique(L)   // lengthy... So we factorize through unique() only for
        // big arrays of strings
        for u = U'
            j = 1:u
            k = find(L==u)
            r(k) = part(txt(k), j(s))
        end
    else                // Otherwise, a direct element-wise processing is faster.
        for i = 1:size(L, "*")
            j = 1:L(i)
            r(i) = part(txt(i), j(s))
        end
    end
endfunction
