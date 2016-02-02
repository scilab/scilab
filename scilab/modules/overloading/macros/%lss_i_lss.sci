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

function s=%lss_i_lss(i,j,s1,s2)
    //%lss_i_lss(i,j,s1,s2) <=>  s2(i,j)=s1
    //!
    if type(i)==10|type(j)==10 then
        error(21)
    end
    [s1 s2]=sysconv(s1,s2)
    [m1,p1]=size(s1(5));[m2,p2]=size(s2(5));

    if size(i)==[-1,-1] then i=1:m2,end
    if size(j)==[-1,-1] then j=1:p2,end
    if max(i)>m2 then
        s2=[s2;0*ones(max(i)-m2,p2)]
        [m2,p2]=size(s2(5));
    end
    if max(j)>p2 then
        s2=[s2,0*ones(m2,max(j)-p2)]
        [m2,p2]=size(s2(5));
    end
    jj=0*ones(1,p2);jj(j)=ones(1,prod(size(j)));
    ii=0*ones(1,m2);ii(i)=ones(1,prod(size(i)));
    s=diag(ones(ii)-ii)*s2*diag(jj)+s2*diag(ones(jj)-jj)
    ii1=0*ones(m2,m1);ii1(m2-m1+1:m2,:)=eye(m1,m1)
    ii1=ii1([find(ii==0),i],:)
    jj1=0*ones(p1,p2);jj1(:,p2-p1+1:p2)=eye(p1,p1)
    jj1=jj1(:,[find(jj==0),j])
    s=s+ii1*s1*jj1
    s=minss(s)
endfunction
