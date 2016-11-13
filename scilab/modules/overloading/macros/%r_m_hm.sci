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

function f1=%r_m_hm(f1,n2)
    // %r_m_s(f1,n2)
    //f1 =  f1*n2
    //author Serge Steer INRIA
    //!

    [n1,d1]=f1(["num","den"]),
    sz1=size(n1);
    if prod(sz1)==0 then return,end
    sz2=size(n2);
    n2=n2(:)

    if or(sz1==-1) then
        n1=n1+0;d1=d1+0;
    end
    if prod(sz1)==1 then
        num=n1*n2,
        den=d1(ones(n2))
    else,
        error(10)
    end,

    f1=rlist(matrix(num,sz2),matrix(den,sz2),f1.dt)
endfunction

