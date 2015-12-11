// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f2=%hm_m_r(n1,f2)
    // %s_m_r(n1,f2)
    //operation  f2=n1*f2
    //author Serge Steer INRIA
    //!
    [n2,d2]=f2(["num","den"]);
    sz2=size(n2);
    if prod(sz2)==0 then  f2=[],return,end
    sz1=size(n1);
    n1=n1(:)

    if  or(sz2==-1) then
        n2=n2+0;d2=d2+0;
    end

    //
    if prod(sz2)==1 then
        num=n1*n2,
        den=d2(ones(n1))
    else,
        error(10)
    end,

    f2=rlist(matrix(num,sz1),matrix(den,sz1),f2.dt)
endfunction
