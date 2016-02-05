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

function M1=%hm_a_s(M1,M2)

    // hypermatrix + scalar

    if and(size(M2)==[1 1]) then
        M1.entries=M1.entries+M2
    else
        dims1=matrix(M1.dims,-1,1)
        dims2=size(M2)';
        dims2(3:size(dims1,"*"))=1;
        if and(dims1==dims2) then //should not occur
            M1.entries=M1.entries+matrix(M2,-1,1)
        else
            error(8)
        end
    end
endfunction
