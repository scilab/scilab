// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M1=%hm_s_s(M1,M2)

    // hypermatrix - scalar

    if and(size(M2)==[1 1]) then
        M1.entries=M1.entries-M2
    else
        dims1=matrix(M1.dims,-1,1)
        dims2=size(M2)';
        dims2(3:size(dims1,"*"))=1;
        if and(dims1==dims2) then //should not occur
            M1.entries=M1.entries-matrix(M2,-1,1)
        else
            error(9)
        end
    end
endfunction
