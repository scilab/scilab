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

function o = obsv_mat(a, c)

    [lhs,rhs]=argn(0)
    select typeof(a)
    case "constant"  then
        if rhs==1 then
            msg = _("%s: Wrong number of input arguments: %d expected")
            error(msprintf(msg, "obsv_mat", 2))
        end
        [m,n] = size(a)
        if m<>n then
           msg = _("%s: Argument #%d: Square matrix expected.\n")
            error(msprintf(msg, "obsv_mat", 1))
        end
        [mb, nb] = size(c);
        if nb<>n then
            msg = _("%s: Arguments #%d and #%d: Same numbers of columns expected.\n")
            error(msprintf(msg, "obsv_mat", 1, 2))
        end
    case "state-space" then
        [a,c]=a([2,4])
        [n,n]=size(a)
    else
        if rhs==1 then
            msg = _("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n")
            error(msprintf(msg,"obsv_mat",1))
        else
            msg = _("%s: Wrong type of input argument #%d: Array of floating point numbers expected.\n")
            error(msprintf(msg, "obsv_mat", 1))
        end
    end;
    o = c;
    for k = 1:n-1
        o = [c ; o*a]
    end
endfunction
