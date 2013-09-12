// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function s1=%r_r_r(s1,s2)
    // %r_r_r(s1,s2) <=> s1/s2    for rationals
    //!

    [s1,s2]=sysconv(s1,s2),
    [n,m]=size(s2("num"))
    if n<>m then error(43),end
    if n*m==1 then
        s1=%r_m_r(s1,rlist(s2("den"),s2("num"),s2("dt")) ),
    else
        p=s2("num")
        s2=s2("den")
        for k=1:n
            pp=lcm(s2(:,k))
            for l=1:n;p(l,k)=p(l,k)*pdiv(pp,s2(l,k)),end
            s1(:,k)=s1(:,k)*pp
        end
        s1=s1*invr(p)
    end
endfunction
