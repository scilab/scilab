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

function M2=%s_l_hm(M1,M2)

    if size(M1,"*")<>1 then
        // M2 = M1\M2   // endless recursive call!
        msg = _("%s: not implemented for argin#1 matrix and argin#2 hypermatrix\n")
        error(msprint(msg, "%s_l_hm"))
    else
        M2 = matrix(M1\M2(:),size(M2))
    end
endfunction
