// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [res1, res2]=%sp_det(A)
    [lhs, rhs]=argn(0);
    hand = umf_lufact(A); //umfpack is used for complex sparse matrix
    [L,U,P,Q,r] = umf_luget(hand);
    res1=prod(r)*prod(diag(U));
    res2=res1;
    if (lhs == 2) then
        res1=0;
        while abs(res2) >= 10
            if abs(res2) < 1 then
                break;
            end
            res2 = res2 / 10;
            res1 = res1 + 1;
        end
    end
    umf_ludel(hand);
endfunction
