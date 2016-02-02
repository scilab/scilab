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

function a=%r_r_s(a,b)
    // a/b a rational matrix, b scalar matrix
    //author Serge Steer
    //!
    if size(b,"*")==0 then a=[],return,end
    [mb,nb]=size(b);
    if mb*nb==1 then
        a.num=a.num/b,
        if or(size(a)==-1)|mb==-1 then
            a.den=a.den*eye(),
        end
        return,
    end
    [ma,na]=size(a.num)
    ma=abs(ma);na=abs(na) //handling eye()
    mb=abs(mb);nb=abs(nb)  //handling eye()
    if na==1 then
        a=rlist(a.num/b,ones(nb,mb)*a.den,a.dt)
    else
        [num,den]=a(["num","den"]);
        dd=[];nn=[]
        for i=1:ma,
            [y,fact]=lcm(den(i,:)),
            nn=[nn;(num(i,:).*fact)/b];
            dd=[dd;y]
        end
        [num,den]=simp(nn,dd*ones(1,mb))
        a=rlist(num,den,a.dt)
    end
endfunction
