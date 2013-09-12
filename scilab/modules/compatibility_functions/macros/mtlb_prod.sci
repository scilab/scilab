// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=mtlb_prod(a,dim)
    // Emulation function for prod() Matlab function

    rhs=argn(2)

    if rhs==1 then
        // Scalar, vector or empty matrix
        if find(size(a)==1)<>[] | find(size(a)==0)<>[] then
            r=prod(a)
        else
            r=prod(a,1)
        end
    else
        if dim<=size(size(a),"*") then
            r=prod(a,dim)
        else
            r=a
        end
    end
endfunction


