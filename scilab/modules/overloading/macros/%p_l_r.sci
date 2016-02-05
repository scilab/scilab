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

function f1=%p_l_r(p1,f1)
    //  f1=p1\f1
    //!

    if size(p1,"*")<>1 then
        f1=invr(p1)*f1
    else
        [n1,d1]=simp(f1("num"),p1*f1("den")),
        f1=rlist(n1,d1,f1("dt"))
    end
endfunction
