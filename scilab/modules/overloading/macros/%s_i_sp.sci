// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function a=%s_i_sp(i,j,b,a)

    [lhs,rhs]=argn(0)
    if rhs==3 then
        a=b;
        b=j;
        [m,n]=size(a)
        a=a(:)
        a(i)=b(:)
        a=matrix(a,m,n)
    end
endfunction
