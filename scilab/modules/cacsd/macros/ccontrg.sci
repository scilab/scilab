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


function [K]=ccontrg(PP,r,Gamma);
    //***********************************
    //   returns a realization of the central controller for the
    //   general problem using the formulas in Gahinet, 92
    //   Note that Gamma must be > gopt (output of gamitg)


    //  PP contains the parameters of plant realization (sylin list)
    //  b = ( b1 , b2 ) ,     c = ( c1 ) ,    d = ( d11  d12)
    //                ( c2 )          ( d21  d22)
    //  r(1) and r(2) are the dimensions of d22 (rows x columns)
    if argn(2)<>3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"ccontrg",3))
    end

    if typeof(PP)<>"state-space" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"ccontrg",1))
    end
    if PP.dt<>"c" then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"ccontrg",1))
    end
    if typeof(r)<>"constant"|~isreal(r) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"),"ccontrg",2))
    end
    if size(r,"*")<>2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: %d expected.\n"),"ccontrg",2,2))
    end
    r=int(r);
    if or(r<=0) then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be positive.\n"),"ccontrg",2))
    end

    //parameter recovery
    [a,b1,b2,c1,c2,d11,d12,d21,d22]=smga(PP,r);

    //dimensions
    [na,na]=size(a); nh=2*na;
    [p1,m2]=size(d12),
    [p2,m1]=size(d21),
    gs=Gamma**2;

    //HAMILTONIAN SETUP
    //------------------
    sh22=m1+m2;
    h11=[a,0*eye(a);-c1'*c1,-a'];
    h12=[-b1,-b2;c1'*d11,c1'*d12];
    h21=[d11'*c1,b1';d12'*c1,b2'];
    h22=gs*eye(m1,m1); h22(sh22,sh22)=0;
    h22=h22-[d11,d12]'*[d11,d12];

    sj22=p1+p2;
    j11=[a',0*eye(a);-b1*b1',-a];
    j12=[-c1',-c2';b1*d11',b1*d21']
    j21=[d11*b1',c1;d21*b1',c2];
    j22=gs*eye(p1,p1); j22(sj22,sj22)=0;
    j22=j22-[d11;d21]*[d11;d21]';



    //computation of Xinf and Yinf
    //-----------------------------

    //compute orthon. bases of the negative inv. subspaces.

    [q,r]=qr([h12;h22]);
    q12=q(1:nh,sh22+1:nh+sh22); q22=q(nh+1:nh+sh22,sh22+1:nh+sh22);
    hr=q12'*h11+q22'*h21;
    [uh,dh]=schur(hr,q12',"c");
    px=uh(1:na,1:na);   qx=uh(na+1:nh,1:na);

    [q,r]=qr([j12;j22]);
    q12=q(1:nh,sj22+1:nh+sj22); q22=q(nh+1:nh+sj22,sj22+1:nh+sj22);
    jr=q12'*j11+q22'*j21;
    [uj,dj]=schur(jr,q12',"c");
    py=uj(1:na,1:na);   qy=uj(na+1:nh,1:na);


    //computation of M,N
    [uz,sz,vz]=svd(qx'*qy/gs-px'*py);
    sz=sqrt(sz);


    //DETERMINATION OF DK
    //-------------------

    [u,s,v]=svd(d12); r12=max(size(find(diag(s) > 1.0e-10)));
    [w,p,z]=svd(d21); r21=max(size(find(diag(p) > 1.0e-10)));
    u1=u(:,1:r12); v1=v(:,1:r12); w1=w(:,1:r21); z1=z(:,1:r21);
    s1=s(1:r12,1:r12); ph1=p(1:r21,1:r21);
    d11tr=u'*d11*z;

    [g0,d0]=parrt(d11tr(r12+1:p1,r21+1:m1),d11tr(r12+1:p1,1:r21),..
    d11tr(1:r12,r21+1:m1),r12,r21);
    dk=v1*s1\(d0-d11tr(1:r12,1:r21))/ph1*w1';


    //DETERMINATION OF BK, CK
    //-----------------------

    hd11=(eye(p1,p1)-u1*u1')*d11;
    hb1=b1-b2*v1*(s1\(u1'*d11));
    that=dk*c2*px+v1*s1\(u1'*c1*px+s1\v1'*b2'*qx+..
    (d0*z1'+u1'*d11*(eye(m1,m1)-z1*z1'))* ..
    ((gs*eye(m1,m1)-hd11'*hd11)\(hb1'*qx+hd11'*c1*px)));

    td11=d11*(eye(m1,m1)-z1*z1');
    tc1=c1-(d11*z1/ph1)*w1'*c2;
    ttil=py'*b2*dk+(py'*b1*z1+qy'*c2'*w1/ph1+..
    ((qy'*tc1'+py'*b1*td11')/(gs*eye(p1,p1)-td11*td11'))* ..
    ((eye(p1,p1)-u1*u1')*d11*z1+u1*d0))/ph1*w1';

    ck=-that*uz/sz; bk=-sz\vz'*ttil;


    //just checking...
    x=qx/px; y=qy/py;
    d12p=pinv(d12);  d21p=pinv(d21);
    thh=d12p*(c1+d12p'*b2'*x)+dk*c2+(d12p*d11+dk*d21)/ ..
    (gs*eye(m1,m1)-hd11'*hd11)*((b1-b2*d12p*d11)'*x+hd11'*c1);
    thh=thh*px;
    ttt=(b1+y*c2'*d21p')*d21p+b2*dk+(y*tc1'+b1*td11')/ ..
    (gs*eye(p1,p1)-td11*td11')*(d11*d21p+d12*dk);
    ttt=py'*ttt;

    nmin=max(norm(hd11),norm(td11));
    ncom=norm(d11+d12*dk*d21);


    //DETERMINATION OF AK
    //-------------------

    ca=a+b2*dk*c2; cb=b1+b2*dk*d21; cc=c1+d12*dk*c2; Cd=d11+d12*dk*d21;
    ak=py'*b2*that+ttil*c2*px-py'*ca*px-qy'*ca'*qx/gs+..
    [-qy'*cc'/Gamma,py'*cb-ttil*d21]/ ..
    [Gamma*eye(p1,p1),Cd;Cd',Gamma*eye(m1,m1)]* ..
    [cc*px-d12*that;-cb'*qx/Gamma];
    ak=sz\(vz'*ak*uz)/sz;



    K=syslin("c",ak,bk,ck,dk);

endfunction

function [go,xo]=parrt(a,b,c,rx,cx);
    //
    // [go,xo]=par(a,b,c,rx,cx) solves the minimization (Parrot) problem:
    //
    //        || a   b ||
    //     min  ||       ||
    //      X   || c   x ||2
    //
    //  an explicit solution is
    //                   2      T  -1  T
    //      Xo = - c ( go  I - a a)   a b
    //  where                    T   T
    //      go = max ( || (a , b) || , || (a , c) || )
    //
    //  rx and cx are the dimensions of Xo (optional if a is nondegenerate)
    //
    //!
    //constant
    TOLA=1.0e-8; // threshold used to discard near singularities in gs I - A'*A

    go=max(norm([a b]),norm([a;c]));
    [ra,cb]=size(b); [rc,ca]=size(c); xo=0;


    //MONITOR LIMIT CASES
    //--------------------
    if ra==0 | ca == 0 | go == 0 then xo(rx,cx)=0; return; end



    //COMPUTE Xo IN NONTRIVIAL CASES
    //------------------------------

    gs=(go**2);
    [u,s,v]=svd(a);

    //form  gs I - s' * s
    ns=min(ra,ca);
    if size(s,1)>1 then
        d=diag(s);
    else
        d=s(1)
    end
    dd=gs*ones(d)-d**2;


    //isolate the non (nearly) singular part of gs I - A'*A
    nnz1=nthresh(dd/go,TOLA);
    nd=ns-nnz1;   //number of singular values thresholded out

    //compute xo
    if nnz1==0 then
        xo(rc,cb)=0;
    else
        unz=u(:,nd+1:ra);
        vnz=v(:,nd+1:ca);
        s=s(nd+1:ra,nd+1:ca);
        for i=1:nnz1
            s(i,i)=s(i,i)/dd(i+nd);
        end
        xo=-c*vnz*s'*unz'*b;
    end
endfunction

function n=nthresh(d,tol)
    n=find(d<=tol,1)
    if n==[] then
        n=size(d,"*"),
    else
        n=n-1
    end
endfunction
