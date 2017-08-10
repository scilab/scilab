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

function [sl,U]=ssrand(nout,nin,nstate,flag)
    //flag=list('co',dim_cont_subs)
    //flag=list('uo',dim_unobs_subs)
    //*******************controllable-unobservable***********
    //display a general Kalman form
    //flag=list('ncno',dim_cno,dim_ncno,dim_co,dim_nco)
    //
    // (A,B,C) U-similar to
    //
    //     [*,*,*,*;     [*;
    //     [0,*,*,*;     [0;
    // A=  [0,0,*,*;  B= [*;
    //     [0,0,0,*]     [0];
    //
    // C=  [0,0,*,*]  D= [0]
    //
    //  (A11,B1) --> controllable, unobservable modes , dimension flag(2)
    //  (A22,B2) --> uncontrolable,unobservables modes, dimension flag(3)
    //  (A33,B3) --> controllable,observable modes , dimension flag(4)
    //  (A44,B4) --> uncontrollable,observable modes, dimension flag(5)
    //*******************stabilizability*********************
    //flag=list('st',dim_cont_subs,dim_stab_subs,dim_stab0)
    //    dim_cont_subs<=dim_stab_subs<=dim_stab0
    //pair (A,B) U-similar to:
    //    [*,*,*,*;     [*;
    //    [0,s,*,*;     [0;
    //A=  [0,0,i,*;   B=[0;
    //    [0,0,0,u]     [0]
    //
    // (A11,B1) controllable  s=stable matrix i=neutral matrix u=unstsble matrix
    //********************detectability***********************
    //flag=list('dt',dim_inst_unob,dim_instb0,dim_unobs)
    //    dim_inst_unob<=dim_instb0<=dim_unobs
    //
    //pair  (A,C) U-similar to:
    //
    //    [u,*,*,*;
    //    [0,i,*,*;
    //A=  [0,0,s,*;
    //    [0,0,0,*]
    //
    //C=  [0,0,0,*]
    //
    // (A44,C4) observable  s=stable matrix i=neutral matrix u=unstsble matrix
    //
    //********************output-nulling***********************
    //flag=list('on',nr,ng,ng0,nv,rk)
    //    nr<=ng<=ng0<=nv<=nstate    and rk<=nu    (rk<nu if nr>0)
    //        nstate-nv>=rk ny>=rk
    //
    //system (A,B,C,D) U-equivalent to
    //
    //    [*,*,*,*,*;     [0,*;
    //    [0,s,*,*,*;     [0,0;
    //A=  [0,0,n,*,*;   B=[0,0;
    //    [0,0,0,u,*;     [0,0;
    //    [*,*,*,*,*]     [*,0]
    //
    //C=  [*,*,*,*,*]   D=[*,0]
    //
    // with (A11,B21) controllable
    //       A22 stable
    //       A33 neutral
    //       A44 unstable
    //       [B51;D11] full column rank
    //       range([A51,A52,A53,A54;C1,C2,C3,C4]) included in range([B51;D11])
    //       dimension A11=nr
    //                 A22=ng-nr
    //                 A33=ng0-ng
    //                 A44=nv-ng0
    //                 A55=nstate-nv
    //       column dimension of B1. = column dimension of D1. =rk
    //**********************'unknown-input'***********************
    //flag=list('ui',nw,nwu,nwui,nwuis,rk)
    //   nw<=nwu<=nwui<=nwuis<=nstate      rk<=nout
    //
    //system (A,B,C,D) U-equivalent to
    //
    //    [*,*,*,*,*;     [*;
    //    [*,u,*,*,*;     [*;
    //A=  [*,0,i,*,*;   B=[*;
    //    [*,0,0,s,*;     [*;
    //    [*,0,0,0,*]     [*]
    //
    //    [0,0,0,0,*;     [0;
    //C=  [*,0,0,0,0]   D=[*;
    //
    //   with (A55,C15) observable
    //         [C21,d2] full row-rank
    //         row-image([[A21;A31;A41;A51]],[B2;B3;B4;B5])
    //  included in row-image([C21,D2]);
    //         A22 unstable
    //         A33 neutral
    //         A44 stable
    //         dimension A11=nw
    //         dimension A22=nwu-nw
    //         dimension A33=nwui-nwu
    //         dimension A44=nwuis-nwui
    //         row dimension C2.= row dimension of D2. =rk
    //***************************************************************

    function [w]=st_able(w,margin)
        if w~=[] then
            w=w-(max(real(spec(w)))+margin)*eye();
        end
    endfunction

    margin=0.5;  //M "stable"  will mean real-part(M) < -margin
    [lhs,rhs]=argn(0)
    //rand('seed',0)
    rand("normal")
    if rhs==3 then flag=[];end
    select flag(1)
    case []
        sl=syslin("c",rand(nstate,nstate),rand(nstate,nin),rand(nout,nstate),..
        zeros(nout,nin),zeros(nstate,1));U=eye(nstate,nstate);
    case "fullV"
        //C^(-1)(D) = X
        rd=flag(2)  //rank(D)   needs rd<nin
        sl=syslin("c",rand(nstate,nstate),rand(nstate,nin),...
        [rand(rd,nstate);zeros(nout-rd,nstate)],[rand(rd,nin);zeros(nout-rd,nin)]);
    case "noW"
        // B ker(D)=0
        rd=flag(2) //rank(D)   needs nout>rd   or C=0
        sl=syslin("c",rand(nstate,nstate),[zeros(nstate,nin-rd),rand(nstate,rd)],...
        rand(nout,nstate),[zeros(nout,nin-rd),rand(nout,rd)]);
    case "fullW"
        //admits polynomial inverse
        iv=rand(nin,nout)
        for k=1:nstate
            iv=iv+(%s^k)*rand(nin,nout);
        end
        sl=minss(inv(tf2ss(iv)));
    case "co"
        nc=flag(2);
        iii=nstate-nc;
        a=[rand(nc,nc),rand(nc,iii);
        zeros(iii,nc),rand(iii,iii)];
        b=[rand(nc,nin);
        zeros(iii,nin)];c=rand(nout,nstate);
        U=rand(nstate,nstate);
        sl=ss2ss(syslin("c",a,b,c),U);
    case "uo"
        no=flag(2)
        iii=nstate-no;
        a=[rand(no,no),rand(no,iii);zeros(iii,no),rand(iii,iii)];
        c=[zeros(nout,no),rand(nout,iii)];b=rand(nstate,nin);
        U=rand(nstate,nstate);
        sl=ss2ss(syslin("c",a,b,c),U);
    case "ncno"
        cno=flag(2);ncno=flag(3);co=flag(4);nco=flag(5);
        a=sysdiag(rand(cno,cno),rand(ncno,ncno),rand(co,co),rand(nco,nco));
        b=[rand(cno,nin);
        zeros(ncno,nin);
        rand(co,nin);
        zeros(nco,nin)];
        c=[zeros(nout,cno+ncno),rand(nout,nstate-cno-ncno)];
        U=rand(nstate,nstate);
        sl=ss2ss(syslin("c",a,b,c),U);
    case "st"
        [nc,ns,ns0]=flag(2:4);nsc=ns-nc;nsc0=ns0-ns;nxs=nstate-ns;
        a=sysdiag(rand(nc,nc),imag_axis(ns-nc,ns0-ns,nstate-ns0)),
        b=[rand(nc,nin);zeros(nstate-nc,nin)];
        c=rand(nout,nstate);
        U=rand(nstate,nstate);
        sl=ss2ss(syslin("c",a,b,c),U);
    case "dt"
        [n1,n2,n3]=flag(2:4);
        w=imag_axis(n3-n2,n2-n1,n1,"uis");
        a=sysdiag(w,rand(nstate-n3,nstate-n3));
        c=[zeros(nout,n3),rand(nout,nstate-n3)];b=rand(nstate,nin);
        U=rand(nstate,nstate);
        sl=ss2ss(syslin("c",a,b,c),U);
    case "on"
        [nr,ng,ng0,nv,rk]=flag(2:6);
        p1=1;p2=1;
        a11=sysdiag(rand(nr,nr),imag_axis(ng-nr,ng0-ng,nv-ng0));
        a12=rand(nv,nstate-nv);
        a21=rand(nstate-nv,nv);a22=rand(nstate-nv,nstate-nv);
        b11=zeros(nv,rk);b12=[rand(nr,nin-rk);zeros(nv-nr,nin-rk)];
        b22=zeros(nstate-nv,nin-rk);d2=zeros(nout,nin-rk);
        c1=rand(nout,nv);c2=rand(nout,nstate-nv);
        b21d1=rand(nstate-nv+nout,rk);
        if rk<>0 then a21c1=b21d1*rand(rk,nv);else a21c1=c1;end
        b21=b21d1(1:nstate-nv,:);d1=b21d1(nstate-nv+1:nstate-nv+nout,:);
        a21=a21c1(1:nstate-nv,:);c1=a21c1(nstate-nv+1:nstate-nv+nout,:);

        if rk>=nout then
            error(msprintf(gettext("%s: Wrong values for input argument #%d: you must choose rk<nout.\n"),"ssrand",4))
        end
        //  if rk>=nout then j=-b21*pinv(d1);a22=-j*c2;end
        [pp,qq]=size(a21);a21(p1+1:pp,1:nv)=0*a21(p1+1:pp,1:nv);
        b21(p1+1:pp,:)=0*b21(p1+1:pp,:);
        [pp,qq]=size(c1);c1(p2+1:pp,:)=0*c1(p2+1:pp,:);
        d1(p2+1:pp,:)=0*d1(p2+1:pp,:);
        w=syslin("c",[a11,a12;a21,a22],[b11,b12;b21,b22],[c1,c2],[d1,d2]);
        U=eye(nstate,nstate);sl=ss2ss(w,U);
    case "ri"
        [nv,nr,nw,rk]=flag(2:5)
        n1=nv-nr;n2=nr;n3=nstate-nv;
        a11=rand(n1,n1);a12=zeros(n1,n2);a21=rand(n2,n1);
        a22=rand(n2,n2);a23=rand(n2,n3);
        a33=rand(n3,n3);
        b2=rand(n2,nin);b31=rand(n3,rk);b32=zeros(n3,nin-rk);
        d1=rand(nout,rk);d2=zeros(nout,nin-rk);
        c3=rand(nout,n3);
        a31=b31*rand(rk,n1);a32=b31*rand(rk,n2);
        c1=d1*rand(rk,n1);c2=d1*rand(rk,n2);
        a13=rand(n1,nout)*c3;b1=rand(n1,nout)*[d1,d2];
        b3=[b31,b32];d=[d1,d2]
        sl=syslin("c",[a11,a12,a13;a21,a22,a23;a31,a32,a33],[b1;b2;b3],[c1,c2,c3],d);
    case "ui"
        [nw,nwu,nwui,nwuis,rk]=flag(2:6)
        a11=rand(nw,nw);
        a22=sysdiag(imag_axis(nwu-nw,nwui-nwu,nwuis-nwui,"uis"),...
        rand(nstate-nwuis,nstate-nwuis))
        a12=rand(nw,nstate-nw);
        b1=rand(nw,nin);
        c21d2=rand(rk,nw+nin);
        a21b2=rand(nstate-nw,rk)*c21d2;
        a21=a21b2(:,1:nw);b2=a21b2(:,nw+1:nw+nin);
        c21=[c21d2(:,1:nw),zeros(rk,nwuis-nw)];
        d2=c21d2(:,nw+1:nw+nin);
        c11=zeros(nout-rk,nwuis);c12=rand(nout-rk,nstate-nwuis);
        d1=zeros(nout-rk,nin);c22=zeros(rk,nstate-nwuis);
        w=syslin("c",[a11,a12;a21,a22],[b1;b2],[c11,c12;c21,c22],[d1;d2]);
        U=rand(nstate,nstate);
        sl=ss2ss(w,U);
    end
    rand("uniform");
    //rand('seed',0)

endfunction
function w=imag_axis(ns,nn,nu,flag);
    //w=random block diagonal matrix with ns stable evals
    //nn imaginary-axis evals and nu unstable evals.
    //flag='uis' blocks along main diagonal appear in the
    // order "unstable", "imaginary axis", "stable".
    [LHS,RHS]=argn(0);
    if RHS==3 then flag="siu";end
    if flag=="siu" then

        function [w]=st_able(w,margin)
            if w~=[] then
                w=w-(max(real(spec(w)))+margin)*eye();
            end
        endfunction

        margin=0.5;  //M "stable"  will mean real-part(M) < -margin
        w=[];k=int(nn/2);
        rand("normal");
        //rand('seed',0);
        w=sysdiag(w,st_able(rand(ns,ns),margin));
        x=rand(1,k);
        for u=x
        w=sysdiag(w,[u,-2*u;u,-u]);end
        if abs(2*k-nn)>1.d-5 then w=sysdiag(w,0);end
        w=sysdiag(w,-st_able(rand(nu,nu),margin));
        rand("uniform");
        //rand('seed',0);
        return
    end
    if flag=="uis" then

        function [w]=st_able(w,margin)
            if w~=[] then
                w=w-(max(real(spec(w)))+margin)*eye();
            end
        endfunction

        w=[];k=int(nn/2);
        rand("normal");
        //rand('seed',0);
        w=sysdiag(w,-st_able(rand(nu,nu),margin));
        x=rand(1,k);
        for u=x
        w=sysdiag(w,[u,-2*u;u,-u]);end
        if abs(2*k-nn)>1.d-5 then w=sysdiag(w,0);end
        w=sysdiag(w,st_able(rand(ns,ns),margin));
        rand("uniform");
        //rand('seed',0);
    end
endfunction
