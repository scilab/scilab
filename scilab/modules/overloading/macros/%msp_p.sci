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

function %msp_p(x)
    [ij,v,mn]=spget(x);
    IJ=string(ij);MN=string(mn);
    if v==[] then disp(v);return;end
    V=string(v);
    n=size(V,1)
    lp="("
    rp=")        "
    comma=",    "

    t=["("+MN(1,1)+comma+MN(1,2)+") m sparse matrix"
    " "
    lp(ones(n,1))+IJ(:,1)+comma(ones(n,1))+IJ(:,2)+rp(ones(n,1))+V(:)]
    write(%io(2),t,"(a)")
endfunction
