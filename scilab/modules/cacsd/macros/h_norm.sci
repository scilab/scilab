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

function [hinfnorm,frequency]=h_norm(Sl,rerr)
    // produces the infinitynorm  of a state-space system
    // (the maximum over all frequencies of the maximum singular value).
    //      [hinfnorm [,frequency]]=h_norm(Sl,rerr)
    //      [hinfnorm [,frequency]]=h_norm(Sl)
    //
    //  Sl        : the state space system (see syslin)
    //  Rerr      : max. relative error, default value 1e-8
    //
    //  hinfnorm  : the infinitynorm of Sl
    //  frequency : frequency at which hinfnorm is achieved
    //  see also: linfn, linf
    //!
    //  Version 3.2, 09-27-1990
    //  Adapted from
    //  N.A. Bruinsma   T.U.Delft/Philips Research Eindhoven, see also
    //  Systems & Control Letters, vol. 14 pp. 287-293.
    if argn(2)<1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),..
        "h_norm",1))
    end

    sltyp=typeof(Sl)
    if and(sltyp<>["rational","state-space"]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n"),..
        "h_norm",1))
    end
    if sltyp=="rational" then Sl=tf2ss(Sl);end
    if argn(2)==1 then
        rerr=1e-8;
    else
        if type(rerr)<>1|size(rerr,"*")<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument: Scalar expected.\n"),"h_norm",2))
        end
        if ~isreal(rerr)|rerr<=0 then
            error(msprintf(gettext( "%s: Input argument #%d must be strictly positive.\n"),"h_norm",2))
        end
    end;

    eps=1.d-8;
    if Sl.dt=="d"|type(Sl.dt)==1 then
        hinfnorm=dhnorm(Sl);frequency=[];
        return;
    end
    [a,b,c,d]=abcd(Sl);
    eiga=spec(a);
    if max(real(eiga)) >= -1e-12 then
        warning(msprintf(_("%s: System is not stable.\n"),"h_norm"))
    end
    if argn(2)==1 then rerr=1e-8; end;
    [no,ns] = size(c); [ns,ni] = size(b);
    if min(ni,no) == 1 then isiso = 2; else isiso = 1; end;
    [p,a] = hess(a); [u,d,v] = svd(d); b = p' * b * v; c = u' * c * p;
    dtd = diag(d'*d); ddt = diag(d*d'); dtc = d' * c;
    aj = sqrt(-1)*eye(ns); R1 = ones(ni,1); S1 = ones(no,1);
    l = [];

    // compute starting value
    q = ((imag(eiga) + 0.01 * ones(eiga)) ./ real(eiga)) ./ abs(eiga);
    [q,i] = max(q); w = abs(eiga(i));
    svw = norm( c * ((w*aj*eye()-a)\b) + d );
    sv0 = norm( -c * (a\b) + d );
    svdd = norm(d);
    [lb,i] = max([svdd sv0 svw]);l=lb;
    w = [1.d30 0 w ]; M = w(i);
    // to avoid numerical problems with Rinv and Sinv if lb == norm(d), lb must be
    // enlarged to at least (1+1e-3)*lb;
    if lb == svdd then lb=1.001*lb+eps;end;
    for it = 1:15,
        gam = (1 + 2 * rerr) * lb; gam2 = gam * gam;
        Rinv = diag(R1./(dtd - gam2 * R1));
        Sinv = diag(S1./(ddt - gam2 * S1));

        H11 = a-b*Rinv*dtc;
        evH = spec([H11 -gam*b*Rinv*b'; gam*c'*Sinv*c  -H11']);
        idx = find(abs(real(evH)) < 1e-8 & imag(evH) >= 0);
        imev= imag(evH(idx));
        [imev] = gsort(imev);
        q = max(size(imev));
        if q <= 1 then
            // q=1 can only happen in the first step if H-norm==maxsv(D) or H-norm==maxsv(0)
            // due to inaccurate eigenvalue computation (so gam must be an upper bound).
            ub = gam;
        else
            M =  0.5 * (imev(1:q-1) + imev(2:q)); M = M(1:isiso:q-1);
            sv=[];
            for j = 1:max(size(M)),
                sv = [sv max(svd(d + c*((M(j)*aj*eye() - a)\b)))];
            end;
            lb = max(sv);l=[l;lb];
        end;
    end;
    if M == 1.d30 then
        lb=svdd;
        warning(msprintf(gettext("%s: norm cannot be computed. Relative accuracy smaller than 1e-3\nHinfnorm is probably exactly max sv(D)\nThe system might be all-pass"),"h_norm"))
    end;
    if exists("ub")==0 then ub=lb;end
    hinfnorm = 0.5 * (ub+lb); frequency = M;

endfunction

function gama=dhnorm(Sl,tol,gamamax)
    //discrete-time case (should be tested!!!)
    //disp('warning: discrete-time h_norm is not fully tested!')
    [lhs,rhs]=argn(0);
    if rhs==1 then tol=0.00001;gamamax=10000000;end
    if rhs==2 then gamamax=1000;end
    gamamin=sqrt(%eps);
    n=0;
    while %T
        gama=(gamamin+gamamax)/2;n=n+1;
        if n>1000 then
            warning(msprintf(gettext("%s: More than %d iterations.\n"),"dhnorm" ,1000));
            return;
        end
        if dhtest(Sl,gama) then
        gamamax=gama; else gamamin=gama
        end
        if (gamamax-gamamin)<tol then return;end
    end

endfunction

function ok=dhtest(Sl,gama)
    //test if discrete hinfinity norm of Sl is < gama
    [A,B,C,D]=abcd(Sl);B=B/sqrt(gama);C=C/sqrt(gama);D=D/gama;
    R=eye()-D'*D;
    [n,n]=size(A);Id=eye(n,n);Z=0*Id;
    Ak=A+B*inv(R)*D'*C;
    e=[Id,-B*inv(R)*B';Z,Ak'];
    Aa=[Ak,Z;-C'*inv(eye()-D*D')*C,Id];
    [As,Es,w,k]=schur(Aa,e,"d");
    //Testing magnitude 1 eigenvalues.
    [al,be]=spec(As,Es);
    finite=find(abs(be)>0.00000001);
    finite_eigen=al(finite)./be(finite);
    bad=find( abs(abs(finite_eigen)-1) < 0.0000001);
    if bad<>[] then ok=%f;return;end
    //if k<>n then ok=%f;return;end
    ws=w(:,1:n);
    x12=ws(1:n,:);
    phi12=ws(n+1:2*n,:);
    if rcond(x12) > 1.d-6 then
        X=phi12/x12;
        z=eye()-B'*X*B
        ok= min(real(spec(z))) > -%eps
    else
    ok=%t;end
endfunction
