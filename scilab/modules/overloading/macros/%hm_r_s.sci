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

function M1=%hm_r_s(M1,M2)

    if size(M2,"*")<>1 then
        // M1 = M1/M2    // endless recursive call!
        msg = _("%s: not implemented for argin#1 hypermatrix and argin#2 matrix\n")
        error(msprint(msg, "%hm_r_s"))
    else
        M1 = matrix(M1(:)/M2, size(M1))
    end
endfunction
