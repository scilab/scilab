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

function f=%r_l_s(f,m)
    //  f\m
    //!

    if size(f("num"),"*")<>1 then
        f=invr(f)*m
    else
        f=rlist(m*f("den"),ones(m)*f("num"),f("dt"))
    end
endfunction
