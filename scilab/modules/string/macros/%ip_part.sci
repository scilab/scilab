// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Samuel GOUGEON
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
    function b = checkip(p)
        b = checkvar(p(1)) & checkvar(p(2)) & checkvar(p(3));
    endfunction

    function b = checkvar(p)
        b = %t;
        if typeof(p) == "polynomial" & varn(p) ~= "$" then
            b = %f;
        end
    endfunction

    //check s
    if ~checkip(s) then
        msg = _("%s: Wrong value for input argument #%d: $ expected.\n");
        error(msprintf(msg,"part",2))
    end

    L = length(txt)
    U = unique(L)
    r = emptystr(txt)
    for u = U(:)'
        j = 1:u
        k = find(L==u)
        r(k) = part(txt(k), j(s))
    end
endfunction
