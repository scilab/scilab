// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) - 2013, 2015, 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
        for u = U(:)'
            k = find(L==u)
            j = horner(s,u)
            j = j(j>0)
            r(k) = part(txt(k), j)
        end
    else                // Otherwise, a direct element-wise processing is faster.
        for i = 1:size(L, "*")
            j = horner(s, L(i))
            j = j(j>0)
            r(i) = part(txt(i), j)
        end
    end
endfunction
