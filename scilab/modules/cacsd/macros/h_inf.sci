// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebecque
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Sk,rk,mu]=h_inf(P,r,mumin,mumax,nmax)
    // H-infinity optimal control for the continuous-time plant P
    // P is the plant (linear system)given in state-space form or in transfer form,
    // e.g. P=syslin('c',A,B,C,D) with A,B,C,D = scalar matrices
    // or P=syslin('c',H) with H a transfer matrix.
    // r = size of the P22 plant i.e. 2-vector [#outputs,#inputs];
    // mumin,mumax = bounds on mu with mu=1/gama^2; (mumin=0  usually)
    // nmax = maximum number of iterations in the gama-iteration.
    // Two possible syntaxes:
    // [Sk,mu]=h_inf(P,r,mumin,mumax,nmax) returns mu and the central controller
    // Sk in the same representation as P. (All calculations being done in state
    // space).
    // [Sk,rk,mu]=h_inf(P,r,mumin,mumax,nmax) returns mu
    //            and the parametrization of all stabilizing controllers:
    //  a stabilizing controller K is obtained by K=Fl(Sk,r,PHI) where
    //  PHI is a linear system with dimensions r' and satisfy h_norm(PHI) < gama.
    //  rk (=r) is the size of the Sk22 block and mu = 1/gama^2 after nmax
    //  iterations.
    //!
    //
    // Initialization : Uci and Yci normalize P
    // mu_inf upper bound on mu = gama^-2
    //P2 = normalized P.
    //
    if argn(2)<>5 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"h_inf",5))
    end

    if and(typeof(P)<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"h_inf",1))
    end
    if P.dt<>"c" then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"h_inf",1))
    end
    if typeof(r)<>"constant"|~isreal(r) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"),"h_inf",2))
    end
    if size(r,"*")<>2 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: %d expected.\n"),"h_inf",2,2))
    end
    r=int(r);
    if or(r<=0) then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be positive.\n"),"h_inf",2))
    end


    if typeof(mumin)<>"constant"|~isreal(mumin) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"),"h_inf",3))
    end
    if size(mumin,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Scalar expected.\n"),"h_inf",3))
    end
    if or(mumin<0) then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be positive.\n"),"h_inf",3))
    end


    if typeof(mumax)<>"constant"|~isreal(mumax) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"),"h_inf",4))
    end
    if size(mumax,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Scalar expected.\n"),"h_inf",4))
    end
    if mumax<0 then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be positive.\n"),"h_inf",4))
    end
    if typeof(nmax)<>"constant"|~isreal(nmax) then
        error(msprintf(gettext("%s: Wrong type for argument #%d: Real vector expected.\n"),"h_inf",5))
    end

    if nmax<>int(nmax)|nmax<=0 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A positive integer expected.\n"),"h_inf",5))
    end
    [P2,mu_inf,Uci,Yci,D22]=h_init(P,r,%t)
    //if mu_inf < mumax then write(%io(2),mu_inf,'(3x,''romax too big: max romax= '',f10.5)');end
    mumax=min(mu_inf,mumax)
    //
    //    Gama-iteration P6 = transformed P2 with D11 removed
    [P6,Finf,mu,Uc#i,Yc#i]=h_iter(P2,r,mumin,mumax,nmax)
    if mu==0 then
        warning(msprintf(_("%s : No feasible ro in bounds [%g  %g]\n"),"h_inf",mumin,mumax));
        rk=[];Sk=[];
        return,
    end
    //
    //    Optimal controller for P6
    [Sk,polesH,polesJ]=h_contr(P6,r,1/mu,Uc#i,Yc#i);
    [E,Ak,Bk1,Bk2,Ck1,Ck2,Dk11,Dk12,Dk21,Dk22]=Sk(:);
    //    Add the optimal controller at infinity
    if norm(Finf,1) <> 0 then Dk11=Dk11+Finf;end
    //    Back-normalization of the controller
    Bk1=Bk1*Yci;
    Ck1=Uci*Ck1;
    Dk11=Uci*Dk11*Yci;
    Dk12=Uci*Dk12;
    Dk21=Dk21*Yci;
    //Convert to descriptor form:
    Sk=des2ss(Ak,[Bk1,Bk2],[Ck1;Ck2],[Dk11,Dk12;Dk21,Dk22],E);
    if argn(1)<3 then
        Sk=Sk(1:r(2),1:r(1));rk=mu;
        //    Case D22 different from zero
        if norm(coeff(D22),1) <> 0 then Sk=Sk/.D22;end
    else
        rk=r;
    end
    //    Sk in transfer representation if P is.
    if typeof(P)=="rational" then Sk=ss2tf(Sk);end;
endfunction

function [P2,mu_inf,Uci,Yci,D22]=h_init(P,r,info)
    //******************************
    // Initialization of the standard plant
    // P = standard plant; r=size of P22 (1X2 vector)
    // P2 = normalized plant
    // gama_inf :lower bound on gama
    //
    //   [C1 D12]'*[C1 D12] = Q = [Q1 S;S' Q2]
    //   [B1;D21] *[B1;D21]'= R = [R1 L';L R2]
    //!

    P1=P(1);
    if P1(1)=="r" then P=tf2ss(P);end
    [A,B1,B2,C1,C2,D11,D12,D21,D22]=smga(P,r);
    [na,na]=size(A);
    [p1,m2]=size(D12),
    [p2,m1]=size(D21),
    //Useful indexes
    l1=1:p1-m2;k1=1:m1-p2;
    l2=1+p1-m2:p1;k2=1+m1-p2:m1;

    //**************Check assumptions******************

    //Stabilizability/detectability of P22 ?

    P22=syslin("c",A,B2,C2);

    [ns,Us,St]=st_ility(P22,1.d-10)

    if ns<>na then
        warning(msprintf(gettext("%s:  %s is not stabilizable.\n"),"h_inf","P22"));
    end
    if ns==na & info then
        mprintf(gettext("%s: %s is stabilizable.\n"),"h_inf","P22");
    end

    [nd,Ud,Sd]=dt_ility(P22,1.d-10)

    if nd <> 0 then
        warning(msprintf(gettext("%s:   %s is not detectable.\n"),"h_inf","P22"));
    end
    if nd==0 & info then
        mprintf(gettext("%s: %s is detectable.\n"),"h_inf","P22");
    end

    // rank P21=[A,B2,C1,D12] = m2 ?
    P12=syslin("c",A,B2,C1,D12);
    [nt,dt]=trzeros(P12),rzt=real(nt./dt),
    if size(nt,"*") > 0 then
        if min(abs(rzt)) < sqrt(%eps) then
            warning(msprintf(gettext("%s: %s has a zero on/close the imaginary axis.\n"),"h_inf","P12")),
        end,
    end,

    // rank P21=[A,B1,C2,D21] = p2 ?
    P21=syslin("c",A,B1,C2,D21);
    [nt,dt]=trzeros(P21),rzt=real(nt./dt),
    if size(nt,"*")>0 then
        if min(abs(rzt)) < sqrt(%eps) then
            warning(msprintf(gettext("%s: %s has a zero on/close the imaginary axis.\n"),"h_inf","P21")),
        end,
    end,

    //***********************end***************************

    //Row compression of D12 (bottom)
    [T1,r1]=rowcomp(D12),
    if r1<>m2 then
        error(msprintf(gettext("%s: Wrong values for input argument #%d:  %s is not full rank.\n"),"h_inf",1,"D12"));
    end,
    T1=[T1(r1+1:p1,:);T1(1:r1,:)],
    D12=T1*D12,
    //Column compression of D21 (right)
    [S1,r2]=colcomp(D21),
    if r2<>p2 then
        error(msprintf(gettext("%s: Wrong values for input argument #%d:  %s is not full rank.\n"),"h_inf",1,"D21"));
    end,
    D21=D21*S1,
    //Updating
    B1=B1*S1,C1=T1*C1,
    D11=T1*D11*S1,

    // Scaling on U and Y
    Uc=D12(l2,:);
    Uci=inv(Uc);
    B2=B2*Uci;
    D12=D12*Uci;

    Yc=D21(:,k2)
    Yci=inv(Yc);
    C2=Yci*C2;
    D21=Yci*D21;

    //P2=[A,B1,B2,C1,C2,D11,D12,D21,D22] with D12 and D21 scaled;

    //Initialization

    //Solve H-infinity problem at infinity

    D1111=D11(l1,k1),
    D1112=D11(l1,k2),
    D1121=D11(l2,k1),
    D1122=D11(l2,k2),

    M11=[D1111,D1112];M22=[D1111',D1121'];
    g1=-1;g2=-1;
    if M11<>[] then g1=norm(M11);end
    if M22<>[] then g2=norm(M22);end

    gama_inf=max(g1,g2);
    if gama_inf==0 then mu_inf=1/%eps/%eps, else mu_inf=1/(gama_inf*gama_inf);end

    P2=syslin("c",A,[B1,B2],[C1;C2],[D11,D12;D21,0*D22]);

    //P2 = standard plant with D22=0 and D12,D21 normalized;


endfunction
function [P6ad,Finfad,muad,Uc#iad,Yc#iad]=h_iter(P2,r,mumin,mumax,nmax)
    niter=0;muad=0;P6ad=[]; Finfad=[];Uc#iad=[];Yc#iad=[];
    while niter < nmax
        niter=niter+1;
        mu=(mumin+mumax)/2;
        [P6,Finf,tv,Uc#i,Yc#i]=h_test(P2,r,mu)

        test=max(tv)

        if test > 0 then
            mumax=mu
        else
            mumin=mu,muad=mu;P6ad=P6;Finfad=Finf;Uc#iad=Uc#i;Yc#iad=Yc#i;
        end

    end  //while
endfunction

function [P6,Kinf,tv,Uc#i,Yc#i]=h_test(P2,r,mu)
    //****************************
    //To test if mu is feasable for the plant P2 :
    //mu is feasible for P2 iff the three components of
    //tv are negative
    //!
    //
    //   [C1 D12]*[C1 D12]'=Q=[Q1 S;S' Q2]
    //   [B1;D21]*[B1;D21]'=R=[R1 L';L R2]
    tv=[1,1,1];
    [A,B1,B2,C1,C2,D11,D12,D21,D22]=smga(P2,r);
    [p1,m2]=size(D12),
    [p2,m1]=size(D21),
    //Useful indexes
    l1=1:p1-m2;k1=1:m1-p2;
    l2=1+p1-m2:p1;k2=1+m1-p2:m1;
    //

    D1111=D11(l1,k1),
    D1112=D11(l1,k2),
    D1121=D11(l2,k1),
    D1122=D11(l2,k2),

    if mu==0 then mu=%eps*%eps;end
    mu1=1/mu;
    gama=1/sqrt(mu);
    gam2=1/(gama*gama);

    err=(m1-p2)*(p1-m2);

    if err==0 then
        Kinf=-D1122
    else
        Kinf=-(D1122+D1121*inv(mu1*eye()-D1111'*D1111)*D1111'*D1112);
    end

    //Kinf=admissible controller for mu

    A=A+B2*Kinf*C2;
    B1=B1+B2*Kinf*D21;
    C1=C1+D12*Kinf*C2;
    D11=D11+D12*Kinf*D21;

    if norm(D11) >= gama then
        P6=[]; Kinf=[];Uc#i=[];Yc#i=[];
        error(msprintf(gettext("%s: gamma too small.\n"),"h_inf"));
    end

    //P3=list(A,B1,B2,C1,C2,D11,D12,D21,D22) with norm(D11) < gama.

    Teta11=gam2*D11;
    Teta22=gam2*D11';
    W12=eye()-gam2*D11*D11';
    W21=eye()-gam2*D11'*D11;
    Teta12=(1/gama)*sqrtm(0.5*(W12+W12'));
    Teta21=-(1/gama)*sqrtm(0.5*(W21+W21'));

    //Teta*Teta'=(1/gama*gama)*eye()

    M=inv(eye()-D11*Teta22);
    N=inv(eye()-Teta22*D11);

    A=A+B1*Teta22*M*C1;

    B2=B2+B1*Teta22*M*D12;
    B1=B1*N*Teta21;

    C2=C2+D21*Teta22*M*C1;
    C1=Teta12*M*C1;

    D11=0*D11;     //By construction...

    D22#=D22+D21*Teta22*M*D12;

    D12=Teta12*M*D12;
    D21=D21*N*Teta21;

    //P4 =syslin('c',A,[B1,B2],[C1;C2],[D11,D12;D21,D22]
    //          with D11=0; P4=Fl(Teta,size(D11'),P3,r);

    D22=0*D22#;

    //P5 = [A,[B1,B2],[C1;C2],[D11,D12;D21,D22] with D11=0 and D22=0;

    //Row compression of D12
    [T1,r1]=rowcomp(D12);
    if r1<>m2 then
        error(msprintf(gettext("%s: Wrong values for input argument #%d:  %s is not full rank.\n"),"h_inf",1,"D12"));
    end
    T1=[T1(r1+1:p1,:);T1(1:r1,:)],
    D12=T1*D12,
    //Column compression of D21
    [S1,r2]=colcomp(D21),
    if r2<>p2 then
        error(msprintf(gettext("%s: Wrong values for input argument #%d:  %s is not full rank.\n"),"h_inf",1,"D21"));
    end,
    D21=D21*S1,
    //Updating
    B1=B1*S1,C1=T1*C1,
    D11=T1*D11*S1,
    // Scaling on U and Y
    Uc#=D12(l2,:);
    Uc#i=inv(Uc#);
    B2=B2*Uc#i;
    D12=D12*Uc#i;

    Yc#=D21(:,k2)
    Yc#i=inv(Yc#);
    C2=Yc#i*C2;
    D21=Yc#i*D21;

    //P6=[A,B1,B2,C1,C2,D11,D12,D21,D22] with D11=0,D22=0;D12 and D21 scaled.
    //Standard assumptions now satisfied

    //P6=[A,B1,B2,C1,C2,D11,D12,D21,D22];

    //     Test of mu for P6  <=> Test of mu^-1 for P2
    //Optimal controller :
    indic=0;

    mu_test=1/mu;

    R1=B1*B1';
    S=D12'*C1;
    C1#=(eye()-D12*D12')*C1;
    Ax=A-B2*S;
    Qx=-C1#'*C1#;

    Rx=mu_test*R1-B2*B2';
    H=[Ax Rx;
    Qx -Ax'];

    dx=min(abs(real(spec(H))));
    //write(%io(2),dx);
    if dx < 1.d-9 then
        mprintf(gettext("%s: An eigenvalue of %s (controller) is close to Imaginary axis.\n"),"h_inf","H");
        write(%io(2),dx);
        indic=1;test=1;
    end
    if indic ==0 then
        [X1,X2,errx]=ric_desc(H);
        if errx > 1.d-4 then
            mprintf(gettext("%s: Riccati solution inaccurate: equation error = %g.\n"),"h_inf",errx);
        end
        //Optimal observer :

        Q1=C1'*C1;
        L=B1*D21';
        B1#=B1*(eye()-D21'*D21);
        Ay=A-L*C2;
        Qy=-B1#*B1#';

        Ry=mu_test*Q1-C2'*C2;

        J=[Ay' Ry;
        Qy -Ay];
        dy=min(abs(real(spec(J))));
        //write(%io(2),dy);
        if dy < 1.d-9 then
            tmp = _("%s: An eigenvalue of %s (observer) is close to Imaginary axis.\nThe distance to the imaginary axis is less than %g ")
            mprintf(tmp,"h_inf","J",1.d-9);
            write(%io(2),dy);
            indic=1 ;test=1;
        end
        if indic==0 then
            [Y1,Y2,erry]=ric_desc(J);
            if erry > 1.d-4 then
                mprintf(gettext("%s: Riccati solution inaccurate: equation error = %g.\n"),"h_inf",erry);
            end
            //Tests
            //
            //     E=(Y2'*X2-mu*Y1'*X1);
            //     write(%io(2),min(svd(E)),'(5x,''min(svd(E)) = '',f10.2)')
            [al1,be1]=spec(A*X2 -B2*(S*X2 +B2'*X1 ),X2);
            [al2,be2]=spec(Y2'*A-(Y2'*L+Y1'*C2')*C2,Y2');
            [al3,be3]=spec(mu_test*Y1'*X1,Y2'*X2);

            //Here division by zero may appear...
            //If such division appear try to uncomment the 3 following lines:
            w1=find(be1==0);be1(w1)=%eps*ones(be1(w1));
            w2=find(be2==0);be2(w2)=%eps*ones(be2(w2));
            w3=find(be3==0);be3(w3)=%eps*ones(be3(w3));

            test1=max(real(al1./be1));
            test2=max(real(al2./be2));
            test3=max(real(al3./be3))-1;
            tv   =[test1,test2,test3]
        end
    end

    //write(%io(2),1/sqrt(mu),'(10x,'' Try gama = '',f18.10)');
    [answer,no]=max(tv);
    //if exists('tv')==1 then write(%io(2),[tv,max(tv)],'(4f15.10)');end
    comm1=_("Unfeasible (Hx hamiltonian)")
    comm2=_("Unfeasible (Hy hamiltonian)")
    comm3=_("Unfeasible (Hy hamiltonian)")
    if exists("tv")==1 then
        if answer>0 then
            // @TODO This stuff should be localized... To bored to understand it for now.
            select no
            case 1  then
                fmt="('' gama = '',f18.10,'' "+_("Unfeasible (Hx hamiltonian)")+"   test = '',e15.5)"
            case 2 then
                fmt="('' gama = '',f18.10,'' "+_("Unfeasible (Hy hamiltonian)")+"   test = '',e15.5)"
            case 3 then
                fmt="('' gama = '',f18.10,'' "+_("Unfeasible (spectral radius)")+"  test = '',e15.5)"
            else
                fmt="('' gama = '',f18.10,''             ok             )  test = '',e15.5)"
            end;
            write(%io(2),[1/sqrt(mu),answer],fmt);
        end
    end
    P6=syslin("c",A,[B1,B2],[C1;C2],[D11,D12;D21,D22])
endfunction

function [Sk,polesH,polesJ]=h_contr(P,r,mu,U2i,Y2i)
    // ****************************
    // Computation of the optimal controller Sk for a standard
    // plant which satisfies the assumption D11=0
    //
    // F.D.
    //!
    //   [C1 D12]*[C1 D12]'=Q=[Q1 S;S' Q2]
    //   [B1;D21]*[B1;D21]'=R=[R1 L';L R2]
    //

    [A,B1,B2,C1,C2,D11,D12,D21,D22]=smga(P,r);
    if norm(D11,1) > %eps then
        error("D11 <> 0"),
    end

    [p2,m1]=size(D21),
    [p1,m2]=size(D12),
    l1=1:p1-m2;k1=1:m1-p2;
    l2=1+p1-m2:p1;k2=1+m1-p2:m1;

    //Initialization  : constants

    R1=B1*B1';
    S=D12'*C1;
    C1#=(eye()-D12*D12')*C1;
    Ax=A-B2*S;
    Qx=-C1#'*C1#;

    Q1=C1'*C1;
    L=B1*D21';
    B1#=B1*(eye()-D21'*D21);
    Ay=A-L*C2;
    Qy=-B1#*B1#';

    //mu-dependent part

    //Optimal controller

    Rx=mu*R1-B2*B2';

    H=[Ax Rx;
    Qx -Ax'];
    polesH=spec(H);
    dx=min(abs(real(polesH)));
    //write(%io(2),dx);
    if dx < 1.d-6 then
        mprintf(gettext("%s: An eigenvalue of %s (controller) is close to Imaginary axis.\n"),"h_inf","H");

    end
    [X1,X2,errx]=ric_desc(H);
    if errx > 1.d-4 then
        mprintf(gettext("%s: Riccati solution inaccurate: equation error = %g.\n"),"h_inf",errx);
    end

    //Optimal observer :

    Ry=mu*Q1-C2'*C2;

    J=[Ay' Ry;
    Qy -Ay];
    polesJ=spec(J);
    dy=min(abs(real(polesJ)));
    //write(%io(2),dy);
    if dy < 1.d-6 then
        mprintf(gettext("%s: An eigenvalue of %s (observer) is close to Imaginary axis.\n"),"h_inf","J");
    end
    [Y1,Y2,erry]=ric_desc(J);
    if erry > 1.d-4 then
        mprintf(gettext("%s: Riccati solution inaccurate: equation error = %g.\n"),"h_inf",erry);
    end

    //Controller in descriptor form

    E=(Y2'*X2-mu*Y1'*X1);
    A#=A-B2*S-L*C2;
    Ak=Y2'*A#*X2+mu*Y1'*A#'*X1-Y2'*(mu*Qy+B2*B2')*X1-Y1'*(mu*Qx+C2'*C2)*X2;
    Bk1=(Y2'*L+Y1'*C2');
    Ck1=-(S*X2+B2'*X1);

    Bk2=Y2'*B2+Y1'*S'
    Ck2=-(C2*X2+L'*X1)

    Dk11=0*Ck1*Bk1;
    Dk22=0*Ck2*Bk2;
    Dk12=eye(Ck1*Bk2);
    Dk21=eye(Ck2*Bk1);

    //Scaling back

    Bk1=Bk1*Y2i;
    Ck1=U2i*Ck1;
    Dk21=Dk21*Y2i;
    Dk12=U2i*Dk12;
    //  Dk11=U2i*Dk11*Y2i

    Sk=list(E,Ak,Bk1,Bk2,Ck1,Ck2,Dk11,Dk12,Dk21,Dk22);
endfunction
