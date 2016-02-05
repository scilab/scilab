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

function [f1]=%r_m_p(f1,n2)
    // r=%r_m_p(f1,n2)  <=> r=f1*n2
    // f1=rational n2=polynomial
    //!

    [n1,d1]=f1(["num","den"]),
    [l1,m1]=size(n1);[l2,m2]=size(n2),
    //
    indef=%f
    if l1==-1 then
        n1=n1+0;d1=d1+0;l1=1;m1=1;
        if l2*m2==1 then indef=%t,else error(14),end
    end
    if l2==-1 then
        n2=n2+0;l2=1;m2=1;
        if l1*m1==1 then indef=%t,else error(14),end
    end
    if min([l1*m1,l2*m2])==1 then,
        num=n1*n2,
        den=d1*ones(l2,m2),
    else,
        if m1<>l2 then error(10),end,
        for i=1:l1,
            n=n1(i,:);
            [y,fact]=lcm(d1(i,:)),
            den(i,1:m2)=ones(1,m2)*y,
            for j=1:m2,
                num(i,j)=n*(n2(:,j).*matrix(fact,l2,1)),
            end,
        end,
    end,
    if ndims(num)<=2 then
        [num,den]=simp(num,den),
    else
        sz=size(num)
        [num,den]=simp(num(:),den(:)),
        num=matrix(num,sz)
        den=matrix(den,sz)
    end
    if indef then
        num=num*eye()
        den=den*eye()
    end
    f1=rlist(num,den,f1("dt"))
endfunction
