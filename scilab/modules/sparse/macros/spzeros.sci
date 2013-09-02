// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [sp]=spzeros(m,n)

    [lhs,rhs]=argn(0)
    if ( rhs < 1 ) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "spzeros" , 1 , 2 ));
    end

    if rhs==1 then
        [m,n]=size(m)
    end
    sp=sparse([],[],[m,n])
endfunction
