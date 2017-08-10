// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
