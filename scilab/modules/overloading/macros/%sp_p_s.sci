// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) BRUNO PINCON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function B=%sp_p_s(A,p)
    // handle A^p for scalar integer p only

    if length(p) ~= 1 then
        error(msprintf(_("%s: Wrong type for input argument #%d: A scalar expected.\n"),"%sp_p_s",2));
    end
    if floor(p) ~= p then
        error(msprintf(_("%s: Wrong type for input argument #%d: An integer expected.\n"),"%sp_p_s",2));
    end
    if p == 0 then
        B = speye(A)
        return
    elseif p < 0 then
        A = inv(A)
        p = -p
    end

    select p
    case 1
        B = A
    case 2
        B = A*A
    case 3
        B = A*(A*A)
    case 4
        B = A*A
        B = B*B
    else
        B = A
        for i=2:p   // slow method (we could use the power algorithm)
            B = A*B
        end
    end

endfunction



