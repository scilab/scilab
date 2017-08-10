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
