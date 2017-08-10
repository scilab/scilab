// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function s=%lss_i_s(i,j,s1,s2)
    //%lss_i_s(i,j,s1,s2) <=> s2(i,j)=s1

    [lhs,rhs]=argn(0)

    if rhs==3 then
        s2=s1;s1=j;
        if (type(i)==10|type(i)==15) then //cell or struct case
            s=createstruct(i,s1)
            if type(i(1))<>10 & i(2)=="entries" then
                // change struct to cell
                f=getfield(1,s);f(1)="ce"
                s=setfield(1,f,s)
            end
            return
        end
        [a1,b1,c1,d1,x1,dom1]=s1(2:7)
        d2=s2;
        [n1,n1]=size(a1);
        if size(d2,2)>1 then
            b2=zeros(1,size(d2,2))
            b2(i)=b1
            c2=c1
        elseif size(d2,1)>1 then
            c2=zeros(size(d2,1),1)
            c2(i)=c1
            b2=b1
        else
            error(msprintf(_("%s: Invalid syntax for a MIMO system.\n"),"%lss_i_s"));
        end
        d2(i)=d1;
        s=lsslist(a1,b2,c2,d2,x1,dom1)
    else
        [a1,b1,c1,d1,x1,dom1]=s1(2:7)
        d2=s2;
        [n1,n1]=size(a1);
        b2(1:n1,j)=b1
        c2(i,1:n1)=c1
        d2(i,j)=d1;
        s=lsslist(a1,b2,c2,d2,x1,dom1)
    end
endfunction
