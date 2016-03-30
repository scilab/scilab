// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [k,x,err]=leqr(p12,vx)
    //h-infinity lqr gain for full-state lq problem
    //(discrete or continuous)
    //
    //                discrete                        continuous
    //   |i  -vx  0|   | a    0     b|       |i   0   0|   | a    vx    b  |
    //  z|0   a'  0| - |-c'c  i    -s|      s|0   i   0| - |-c'c  -a'  -s  |
    //   |0   b'  0|   | s'   0   d'd|       |0   0   0|   | s'   -b'   d'd|
    //

    [lhs,rhs]=argn(0);

    if typeof(p12)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"leqr",1))
    end
    [a,b2,c1,d12]=p12(2:5);
    [n,nu]=size(b2);
    [ny,n]=size(c1);
    dom=p12(7);
    if dom==[] then
        dom="c";
        warning(msprintf(gettext("%s: Input argument %d is assumed continuous time.\n"),"leqr",1));
    end
    select dom
        //      continuous
    case "c" then
        z=0*a;i=eye(a);
        q=c1'*c1;r=d12'*d12;s=c1'*d12;
        bige=[i,z,zeros(b2);
        z,i,zeros(b2);
        zeros(nu,2*n+nu)];
        biga=[a,vx,b2;
        -q,-a',-s;
        s',b2',r];

        [bige,biga,dummy,z]=balanc(bige,biga);
        [w,k]=schur(biga,bige,"c");
        if k<>n then
            warning(msprintf(gettext("%s: Stable subspace is too small.\n"),"leqr"));
            k=[];w=[];err=[];
            return;
        end

        ws=z*w(:,1:n);
        x12=ws(1:n,:);
        if rcond(x12) < 1.d-6 then
            warning(msprintf(gettext("%s: Bad conditionning.\n"),"leqr"));
        end
        k=ws(2*n+1:2*n+nu,:)/x12;
        x=ws(n+1:2*n,:)/x12;
        if lhs~=3 then return;end
        ri=pinv(r);
        err=norm((a-b2*ri*s')'*x+x*(a-b2*ri*s')-x*(b2*ri*b2'-vx)*x+q-s*ri*s',1)
        //k=-ri*(b2'*x+s')
        //      discrete  time
    case "d" then
        i=eye(a);z=0*i;
        q=c1'*c1;r=d12'*d12;s=c1'*d12;
        bige=[i,-vx,zeros(b2);
        z,a',zeros(b2);
        zeros(b2'),-b2',zeros(b2'*b2)];
        biga=[a,z,b2;
        -q,i, -s;
        s', 0*b2', r];
        [bige,biga,dummy,z]=balanc(bige,biga);
        [w,k]=schur(biga,bige,"d");
        if k<>n then
            warning(msprintf(gettext("%s: Stable subspace is too small.\n"),"leqr"));
            k=[];w=[];err=[];
            return;
        end
        ws=z*w(:,1:n);
        x12=ws(1:n,:);
        if rcond(x12) <1.d-6 then
            warning(msprintf(gettext("%s: Bad conditionning.\n"),"leqr"));
            k=[];w=[];
            return;
        end

        k=ws(2*n+1:2*n+nu,:)/x12;
        x=ws(n+1:2*n,:)/x12;
        if norm(x-x',1)>0.0001 then
            warning(msprintf(gettext("%s: %s non symmetric.\n"),"leqr","x"));
            k=[];w=[];
            return;
        end

        //a'*x*a-(a'*x*b2+c1'*d12)*pinv(b2'*x*b2+d12'*d12)*(b2'*x*a+d12'*c1)+c1'*c1
        if lhs~=3 then return;end
        ri=pinv(r);
        abar=a-b2*ri*s';
        qbar=q-s*ri*s';
        err=norm(x-(abar'*inv((inv(x)+b2*ri*b2'-vx))*abar+qbar),1);
        //k=-ri*(b2'*inv(inv(x)+b2*ri*b2'-vx)*abar+s')
    end
endfunction
