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

function f=%p_r_r(m,f)
    // f=%p_r_r(p1,p2) <=> f= p1*(p2^(-1))
    // p1 polynomial matrix
    // p2: rational matrix
    //!

    if prod(size(f("num")))<>1 then f=m*invr(f),return,end
    f=rlist(m*f("den"),ones(m)*f("num"),f("dt"))
endfunction
