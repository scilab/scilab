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

function h=%p_v_r(h1,h2)
    // h=%p_v_r(h1,h2) <=> (I+h1*h2)\h1.
    // h1 polynomial matrix
    // h2 rational matrix
    //!

    [m1,n1]=size(h1)
    [m2,n2]=size(h2("num"))
    if abs(n1-m2)+abs(m1-n2)<>0 then error(msprintf(_("%s: Wrong size for input argument(s).\n"),"%p_v_r")),end
    h=(eye(m1,m1)+h1*h2)\h1
endfunction
