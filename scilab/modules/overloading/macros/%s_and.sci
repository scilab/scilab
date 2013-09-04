// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function x=%s_and(a,flag)
    // or(a) returns the logical OR
    // for scalar matrices, an entry is TRUE if it is not zero.
    //!

    if argn(2)==1 then flag="*",end
    if flag=="*" then
        x=find(a==0,1)==[]
    else
        if a==[] then x=[],return,end
        x=sum(bool2s(a),flag)==size(a,flag)
    end
endfunction
