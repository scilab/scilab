// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) BRUNO PINCON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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



