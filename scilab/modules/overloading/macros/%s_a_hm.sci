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
function M2=%s_a_hm(M1,M2)

    // hypermatrix + scalar
    if and(size(M1)==[1 1]) then
        M2.entries=M1+M2.entries
    else
        dims1=size(M1)';
        dims2=matrix(M2.dims,-1,1)
        dims1(3:size(dims2,"*"))=1
        if and(dims1==dims2) then //should not occur
            M2.entries=matrix(M1,-1,1)+M2.entries
        else
            error(8)
        end
    end
endfunction
