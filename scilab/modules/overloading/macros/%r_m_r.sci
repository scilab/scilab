// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function s1=%r_m_r(s1,s2)
    //s1=s1*s2
    //!

    [s1,s2]=sysconv(s1,s2),
    [n1,d1]=s1(["num","den"]);
    [n2,d2]=s2(["num","den"]);
    sz1=size(n1)
    sz2=size(n2)

    indef=%f
    if or(sz1==-1) then
        n1=n1+0;d1=d1+0;sz1=[1 1]
        if prod(sz2)==1 then indef=%t,else error(14),end
    end
    if or(sz2==-1) then
        n2=n2+0;d2=d2+0;sz2=[1 1]
        if  prod(sz1)==1 then indef=%t,else error(14),end
    end
    if prod(sz1)==1 then
        [num,den]=simp(n1.*n2(:),d1.*d2(:)),
        num=matrix(num,sz2)
        den=matrix(den,sz2)
    elseif prod(sz2)==1 then
        [num,den]=simp(n1(:).*n2,d1(:).*d2),
        num=matrix(num,sz1)
        den=matrix(den,sz1)
    else
        if size(sz1,"*")>2|size(sz2,"*")>2 then error(10),end
        if sz1(2)<>sz2(1) then error(10),end,
        l1=sz1(1);m1=sz1(2);m2=sz2(2);
        for i=1:l1, pp(i)=lcm(d1(i,:)),end,
        for j=1:m2,
            y=lcm(d2(:,j)),
            for i=1:l1,
                yij=y*pp(i),
                x=0;
                for k=1:m1,
                    x=x+n1(i,k)*n2(k,j)*pppdiv(y,d2(k,j))*pppdiv(pp(i),d1(i,k)),
                end
                num(i,j)=x,den(i,j)=yij,
            end
        end
        [num,den]=simp(num,den),
    end

    if indef then
        num=num*eye()
        den=den*eye()
    end
    s1=rlist(num,den,s1.dt)
endfunction
