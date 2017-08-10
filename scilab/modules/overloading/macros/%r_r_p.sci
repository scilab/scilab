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

function f=%r_r_p(f1,p2)
    //
    //!

    if prod(size(p2)) <>1 then f=f1*invr(p2),return,end
    [n1,p2]=simp(f1("num"),p2*f1("den"))
    f=rlist(n1,p2,f1("dt"))
endfunction
