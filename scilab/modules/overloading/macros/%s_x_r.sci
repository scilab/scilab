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

function f2=%s_x_r(n1,f2)
    // %s_x_r(M,r) = (M.*r)   constant .* rational
    //!

    if size(n1,"*")==0 then
        f2=[]
    else
        f2("num")=n1.*f2("num")
    end
endfunction
