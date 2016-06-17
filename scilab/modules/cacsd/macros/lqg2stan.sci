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

function [P,m]=lqg2stan(P22,Q,R)
//P = standard plant for LQG control problem
//  described by the triple (A,B,C)
//  .
//  x = Ax + w1 + Bu   (resp x[n+1]= ... if dom='d')
//  y = Cx + w2
//
//  w1 and w2 white noises with unit variance and cov(w1,w2)=R;
//
//  mininize (x,u)'Q(x,u)
//

    flag=0;
    if typeof(P22)=="rational" then
      P22=tf2ss(P22);
      flag=1;
    elseif typeof(P22)=="constant" then
      P22=syslin("c",[],[],[],1)
    end
    if typeof(P22)<>"state-space" then 
      error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"lqg2stan",1))
    end
    //P22=-P22;//bug 13751 fix
    [ny,nu,nx]=size(P22);
    if or(size(Q)<>nx+nu) then
      error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),...
                       "lqg2stan",2,nx+nu,nx+nu))
    end
    if or(size(R)<>nx+ny) then
      error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),...
                       "lqg2stan",3,nx+ny,nx+ny))
    end
    if norm(Q.'-Q,1)>100*%eps*norm(Q,1) then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be symmetric.\n"),"lqg2stan",2))
    end
    if norm(R.'-R,1)>100*%eps*norm(R,1) then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be symmetric.\n"),"lqg2stan",3))
    end
    dom=P22.dt;
    if dom==[] then
        warning(msprintf(gettext("%s: Input argument #%d is assumed continuous time.\n"),"lqg2stan",1));
        dom="c";
    end
    Qhalf=sqrtm(Q);
    if norm(imag(Qhalf),1)>100*%eps then
       error(msprintf(_("%s: Wrong value for input argument #%d: Must be a symmetric non negative matrix.\n"),"lqg2stan",2))
    end
    Qhalf=real(Qhalf);
    Rhalf=sqrtm(R);
    if norm(imag(Qhalf),1)>100*%eps then
       error(msprintf(_("%s: Wrong value for input argument #%d: Must be a symmetric non negative matrix.\n"),"lqg2stan",3))
    end
    Rhalf=real(Rhalf);
    
    B1=Rhalf(1:nx,:);D21=Rhalf(nx+1:nx+ny,:);
    C1=Qhalf(:,1:nx);D12=Qhalf(:,nx+1:nx+nu);
    D11=0*C1*B1;

    P=syslin(dom,P22.A,real([B1,P22.B]),real([C1;P22.C]),real([D11,D12;D21,P22.D]));
    m=[ny,nu];
    if flag==1 then
      P=ss2tf(P);
    end
endfunction
