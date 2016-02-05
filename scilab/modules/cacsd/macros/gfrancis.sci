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

function [L,M,T]= gfrancis(Plant,Model);
    // This function computes an optimal model matching
    // controller for the linear plant
    //    x'= F*x + G*u
    //    y = H*x + J*u
    // and the linear model
    //    xm'= A*xm + B*um
    //    ym = C*xm + D*um
    // The dimensions of x,u,y are n,m,p and those of xm,um,ym are
    // nn,mm,pp and pp=p.
    // The goal is for the plant to track the model
    //    e = y - ym ---> 0
    // while keeping stable the state x(t) of the plant. To accomplish
    // this, we use feedforward and feedback
    //    u = L*xm + M*um + K*(x-T*xm) = [K , L-K*T] *(x,xm) + M*um
    // to drive the combined system to the closed loop invariant subspace
    //    x = T*xm
    // where e = 0.
    // The matrices T,L,M satisfy generalized Francis equations
    //    F*T + G*L = T*A
    //    H*T + J*L = C
    //          G*M = T*B
    //          J*M = D
    // The matrix K is chosen as stabilizing the pair (F,G) i.e
    // F+G*K is stable.
    // For more information on this approach, see
    // Krener, A. J., Optimal model matching controllers for linear
    // and nonlinear systems, Proceedings of NOLCOS, Bordeaux, 1992.
    if typeof(Plant)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"gfrancis",1))
    end
    if Plant.dt<>"c" then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"gfrancis",1))
    end
    if typeof(Model)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"gfrancis",2))
    end
    if Model.dt<>"c" then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"gfrancis",2))
    end

    [F,G,H,J]=abcd(Plant);
    [A,B,C,D]=abcd(Model);
    [nf,nf]=size(F);[ny,nu]=size(J);
    [na,na]=size(A);[lc,num]=size(D);
    Ia=eye(na,na);Inf=eye(nf,nf);Iu=eye(num,num);
    Mat=[Ia.*.F-A'.*.Inf, Ia.*.G, zeros(nf*na,nu*num);
    Ia.*.H , Ia.*.J, zeros(ny*na,nu*num);
    -B'.*.Inf, zeros(nf*num,nu*na), Iu.*.G;
    zeros(ny*num,nf*na),zeros(ny*num,nu*na),Iu.*.J];

    rhs=[zeros(nf*na,1);
    matrix(C,size(C,"*"),1);
    zeros(nf*num,1);
    matrix(D,size(D,"*"),1)];
    TLM=pinv(Mat)*rhs;
    T=TLM(1:nf*na);T=matrix(T,nf,na);
    L=TLM(nf*na+1:nf*na+nu*na);L=matrix(L,nu,na);
    M=TLM(nf*na+nu*na+1:nf*na+nu*na+nu*num);M=matrix(M,nu,num);
    Wplant=[F,G;H,J];
    Wmodel=[A,B;C,D];
    //check
    err=norm(Wplant*[T,zeros(nf,num);
    L,M]-[T,zeros(nf,lc);
    zeros(lc,na),eye(lc,lc)]*Wmodel,1);
    if err > 1.d-5 then warning(msprintf(gettext("%s: Francis equations not satisfied.\n"),"gfrancis"));end
endfunction
