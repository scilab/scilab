// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function s=%s_a_sp(a,b)
    // %s_a_sp - adds a  scalar a and a sparse matrix b
    //!

    if size(a)==[-1,-1] then
        //eye+b
        [m,n]=size(b)
        s=(0+a)*speye(m,n)+b
    else
        s=a+full(b)
    end
endfunction
