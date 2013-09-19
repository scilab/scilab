// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [z]=%s_q_p(x,y)
    //z = x .\ y special cases
    // and other matrix polynomial

    [m,n]=size(x)
    if m*n==1 then
        z=x*ones(y).\y
    else
        z=x.\y*ones(x)
    end
endfunction
