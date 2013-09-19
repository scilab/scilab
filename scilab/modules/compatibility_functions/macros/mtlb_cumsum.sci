// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function B=mtlb_cumsum(A,dim)
    // Emulation function for Matlab cumsum()

    [lhs,rhs]=argn()

    if rhs==1 then
        if size(a,1)==1|size(a,2)==1 then
            r=cumsum(a)
        else
            r=cumsum(a,firstnonsingleton(A))
        end
    else
        if dim<=size(size(A),"*") then
            B=cumsum(A,dim)
        else
            B=A
        end
    end
endfunction
