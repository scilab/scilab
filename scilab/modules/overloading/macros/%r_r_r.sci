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
