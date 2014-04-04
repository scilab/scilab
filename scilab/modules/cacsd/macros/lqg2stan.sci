// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [P,m]=lqg2stan(P22,Q,R)
    //P = standard plant for LQG control problem
    //described by the triple (A,B,C)
    //  .
    //  x = Ax + w1 + Bu   (resp x[n+1]= ... if dom='d')
    //
    //  y = Cx + w2
    //
    //  cov(w1,w2)=R;
    //
    //  mininize (x,u)'Q(x,u)
    //

    flag=0;
    P221=P22(1);
    if P221(1)=="r" then
    P22=tf2ss(P22);flag=1;end
    P22=-P22;
    [A,B,C,D22]=P22(2:5);
    [nx,nu]=size(B);
    [ny,nx]=size(C);
    Qhalf=real(sqrtm(Q));
    Rhalf=real(sqrtm(R));
    B1=Rhalf(1:nx,:);D21=Rhalf(nx+1:nx+ny,:);
    B2=B;
    C1=Qhalf(:,1:nx);D12=Qhalf(:,nx+1:nx+nu);
    C2=C;
    D11=0*C1*B1;
    dom=P22(7);
    if dom==[] then
        warning(msprintf(gettext("%s: Input argument #%d is assumed continuous time.\n"),"lqg2stan",1));
    end
    P=syslin(dom,A,real([B1,B2]),real([C1;C2]),real([D11,D12;D21,D22]));
    m=size(C2*B2);
    if flag==1 then
    P=ss2tf(P);end
endfunction
