// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
