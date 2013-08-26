// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%r_r_p(f1,p2)
    //
    //!

    if prod(size(p2)) <>1 then f=f1*invr(p2),return,end
    [n1,p2]=simp(f1("num"),p2*f1("den"))
    f=rlist(n1,p2,f1("dt"))
endfunction
