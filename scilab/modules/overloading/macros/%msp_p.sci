// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
