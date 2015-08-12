// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f1=%r_x_p(f1,f2)
    // %r_x_p(r,p)=r.*p
    //author Serge Steer, INRIA
    //!
    f1.num=f1.num.*f2
    sz=size(f1.num)
    if size(sz,"*")<=2 then

        f1=simp(f1)
    else
        [num,den]=simp(f1.num.entries,f1.den.entries)
        f1.num=matrix(num,sz)
        f1.den=matrix(den,sz)
    end
endfunction
