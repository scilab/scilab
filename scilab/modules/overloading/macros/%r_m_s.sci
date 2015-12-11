// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f1=%r_m_s(f1,n2)
    // %r_m_s(f1,n2)
    //f1 =  f1*n2
    //author Serge Steer INRIA
    //!

    [n1,d1]=f1(["num","den"]),
    sz1=size(n1);sz2=size(n2);
    if prod(sz1)==0|prod(sz2)==0 then return,end


    indef=%f
    if or(sz1==-1) then
        n1=n1+0;d1=d1+0;sz1=[1 1];
        if prod(sz2)==1 then indef=%t,else error(14),end
    end
    if  or(sz2==-1) then
        n2=n2+0;sz2=[1 1];
        if prod(sz1)==1 then indef=%t,else error(14),end
    end
    //
    if prod(sz1)==1 then
        num=n1*n2,
        den=d1(ones(n2))
    elseif prod(sz2)==1 then
        num=n1*n2,
        den=d1
    else,
        if size(sz1,"*")>2|size(sz2,"*")>2 then error(10),end
        if sz1(2)<>sz2(1) then error(10),end,
        l1=sz1(1);l2=sz2(1);m2=sz2(2)
        for i=1:l1,
            n=n1(i,:);
            [y,fact]=lcm(d1(i,:)),
            den(i,1:m2)=ones(1,m2)*y,
            for j=1:m2,
                num(i,j)=n*(n2(:,j).*matrix(fact,l2,1)),
            end,
        end,
        [num,den]=simp(num,den),
    end,

    if indef then
        num=num*eye()
        den=den*eye()
    end
    f1=rlist(num,den,f1.dt)
endfunction
