// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%p_r_p(p1,p2)
    // f=%p_r_p(p1,p2) <=> f= p1*(p2^(-1))
    //p1 polynomial matrix
    //p2 polynomial matrix
    //!

    [l,c]=size(p2)
    if l*c <>1 then f=p1*invr(p2),return,end
    [l,c]=size(p1)
    [p1 p2]=simp(p1,p2*ones(l,c))
    f=rlist(p1,p2,[])
endfunction
