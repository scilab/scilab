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

function [UIobs,J,N]=ui_observer(Sys,reject,C1,D1,flag,Alfa,Beta)
    // ------------Unknown input observer------------------
    // Sys: (w,u) --> y  = (A,B,C2,D2) syslin linear system with two inputs
    // w and u, w being the unknown input.
    // The  matrices B and D2 of Sys are (implicitly) partitioned as:
    // B=[B1,B2] and D2=[D21,D22]  with B1=B(:,reject)  and D21=D2(:,reject)
    // where reject = indices of unknown inputs.
    // The matrices C1 and D1 define
    // z = C1 x + D1 (w,u) = the to-be-estimated output
    // the matrix D1 is (implicitly) partitioned as
    // D1=[D11,D12] with D11=D(:,reject)
    //
    // The data (Sys, reject,C1, D1)   define a 2-input 2-output system:
    //
    //  xdot =  A x + B1  w + B2  u
    //     z = C1 x + D11 w + D12 u
    //     y = C2 x + D21 w + D22 u
    //
    //  An observer (u,y) --> zhat is looked for the output z.
    // flag='ge' no stability constraints
    //     ='st' stable observer    (default)
    //     ='pp' observer with pole placement
    //     Alfa,Beta = desired location of closed loop poles  (default -1, -2)
    // UIobs = linear system (u,y) --> zhat such that:
    // The transfer function: (w,u) --> z  equals the composed transfer function:
    //            [0,I;        UIobs
    //             Sys]
    //      (w,u) -----> (u,y) -----> zhat
    // i.e. transfer function of system {A,B,C1,D1} equals
    // transfer function UIobs*[0,I; Sys]
    // J=y-output to x-state injection
    // N=y-output to z-estimated output injection
    // Example:
    //A=diag([3,-3,7,4,-4,8]);
    //B=[eye(3,3);zeros(3,3)];
    //C=[0,0,1,2,3,4;0,0,0,0,0,1];
    //D=[1,2,3;0,0,0];
    //rand('seed',0);w=ss2ss(syslin('c',A,B,C,D),rand(6,6));
    //[A,B,C,D]=abcd(w);
    //B=[B,matrix(1:18,6,3)];D=[D,matrix(-(1:6),2,3)];
    //reject=1:3;
    //Sys=syslin('c',A,B,C,D);
    //N1=[-2,-3];C1=-N1*C;D1=-N1*D;
    //nw=length(reject);nu=size(Sys('B'),2)-nw;
    //ny=size(Sys('C'),1);nz=size(C1,1);
    //[UIobs,J,N]=ui_observer(Sys,reject,C1,D1);
    //
    //W=[zeros(nu,nw),eye(nu,nu);Sys];UIobsW=UIobs*W;
    //(w,u) --> z=UIobs*[0,I;Sys](w,u)
    //clean(ss2tf(UIobsW));
    //wu_to_z=syslin('c',A,B,C1,D1);clean(ss2tf(wu_to_z));
    //clean(ss2tf(wu_to_z)-ss2tf(UIobsW),1.d-7)
    //2nd ex: nx=2;ny=3;nwu=2;Sys=ssrand(ny,nwu,nx);
    //        C1=rand(1,nx);D1=[0,1];
    //        UIobs=ui_observer(Sys,1,C1,D1);
    [LHS,RHS]=argn(0);
    if RHS==6 then Beta=-1;end
    if RHS==5 then Beta=-1;Alfa=-1;end
    if RHS==4 then Beta=-1;Alfa=-1;flag="st";end
    if RHS==3 then Beta=-1;Alfa=-1;flag="st";D1=[];end
    if size(C1,2) ~= size(Sys("A"),1) then
        msg = _("%s: Incompatible input arguments #%d and #%d: state dimension of #%d must be equal to the column dimension of #%d.\n")
        error(msprintf(msg, "ui_observer", 1, 3, 1, 3))
    end
    if size(D1,2) ~= size(Sys("B"),2) then
        msg = _("%s: Incompatible input arguments #%d and #%d: input dimension of #%d must be equal to the column dimension of #%d.\n")
        error(msprintf(msg, "ui_observer", 4, 1, 4, 1))
    end
    not_reject=1:size(Sys,"c");not_reject(reject)=[];
    Sys1=Sys(:,reject);      //A,B1,C2,D21
    [X,dims,J,Y,k,Z]=cainv(Sys1,Alfa,Beta,flag);
    Sys_new=ss2ss(Sys,X);
    ns=dims(3);Jnew=X'*J;J2=Jnew(ns+1:$,:);
    [Anew,Bnew,Cnew,Dnew]=abcd(Sys_new);
    A22=Anew(ns+1:$,ns+1:$);C22=Cnew(:,ns+1:$);
    B22=Bnew(ns+1:$,not_reject);D22=Dnew(:,not_reject);
    Sys22=syslin(Sys("dt"),A22,B22,C22,D22);
    UIobs=observer(Sys22,J2);
    //
    C1new=C1*X;
    D11=D1(:,reject);D21=Dnew(:,reject);
    C11=C1new(:,1:ns);C21=Cnew(:,1:ns);
    //N s.t. [I,N]*[C11 D11;C21 D21]=[0,0]
    N=lowlevel();
    mprintf(gettext("%s: Residual norm = %g\n"),"ui_observer",norm([eye(size(N,1),size(N,1)),N]*[C11,D11;C21,D21]));
    D12=D1(:,not_reject);C12=C1new(:,ns+1:$);
    UIobs("C")=[C12+N*C22];UIobs("D")=[D12+N*D22,-N];

endfunction

function N=lowlevel()
    ww=[C11 D11;C21 D21];
    [xx,dd]=rowcomp(ww);
    K=xx(dd+1:$,:);
    colN=size(C21,1);rowN=size(C11,1);
    if size(K,1) > rowN then K=K(1:rowN,:);end
    Kleft=K(:,1:size(K,1))
    if size(Kleft,"*")==1 & abs(Kleft) <1.d-8 then
        N=[];
        error(msprintf(gettext("%s: Bad conditionning.\n"),"ui_observer"));
    end
    if rcond(Kleft) <= 1.d-10 then
        warning(msprintf(gettext("%s: Bad conditionning.\n"),"ui_observer"));
        K1=pinv(Kleft)*K;N=K1(:,size(K,1)+1:$);return
    end
    K1=inv(Kleft)*K;   //test conditioning here!
    N=K1(:,size(K,1)+1:$)
endfunction
