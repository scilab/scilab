// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function o=obsv_mat(a,c)

    [lhs,rhs]=argn(0)
    select typeof(a)
    case "constant"  then
        if rhs==1 then
            error(msprintf(gettext("%s: Wrong number of input arguments: %d expected"),"obsv_mat",2)),
        end
        [m,n]=size(a)
        if m<>n then error(20,1),end
        [mb,nb]=size(c);if nb<>n then error(60),end
    case "state-space" then
        [a,c]=a([2,4])
        [n,n]=size(a)
    else
        if rhs==1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n"),"obsv_mat",1))
        else
            error(msprintf(gettext("%s: Wrong type of input argument #%d: Array of floating point numbers expected.\n"),"obsv_mat",1))
        end
    end;
    o=c;for k=1:n-1, o=[c;o*a],end
endfunction
