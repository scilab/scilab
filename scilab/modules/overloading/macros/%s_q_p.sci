// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
