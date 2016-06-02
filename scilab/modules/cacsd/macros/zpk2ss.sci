// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function s=zpk2ss(sys)
    //zero pole gain to state space (simo system)

    if typeof(sys)<>"zpk" then
        error(msprintf(_("%s: Wrong type for input argument #%d: zero-pole-gain representation expected.\n"),"zpk2ss",1))
    end
    [ny,nu]=size(sys)
    siso=ny*nu==1


    if siso then
        s=zp2ss_siso(sys.Z{1},sys.P{1},sys.K,sys.dt);
    else
        s=[];
        for l=1:ny
            for k=1:nu
                s(l,k)=zp2ss_siso(sys.Z{l,k},sys.P{l,k},sys.K(l,k),sys.dt);
            end
        end
        s=minss(s);
    end
endfunction

function s=zp2ss_siso(Z,P,K,dt)
    //remove poles and zeros at infinity
    Z(Z==%inf)=[];
    P(P==%inf)=[];

    //Split real and complex
    Zr=real(Z(imag(Z)==0));nZr=size(Zr,"*");
    Zc=Z(imag(Z)<0);nZc=size(Zc,"*");

    Pr=real(P(imag(P)==0));nPr=size(Pr,"*");
    Pc=P(imag(P)<0);nPc=size(Pc,"*");

    //real (zero pole) pairs
    nr=min(nZr,nPr)
    s=syslin(dt,[],[],[],1);
    for k=1:nr
        s=s*syslin(dt,Pr(k),1,Pr(k)-Zr(k),1);
    end
    Zr=Zr(nr+1:$);nZr=nZr-nr;
    Pr=Pr(nr+1:$);nPr=nPr-nr;
    //complex (zero pole) pairs
    nc=min(nZc,nPc) ;
    for k=1:nc
        wn=abs(Pc(k));
        A=[2*real(Pc(k)) -wn;wn 0];
        B=[1;0];
        C=[2*real(Pc(k)-Zc(k)), (abs(Zc(k))^2-wn^2)/wn]
        s=s*syslin(dt,A,B,C,1);
    end
    Zc=Zc(nc+1:$);nZc=nZc-nc;
    Pc=Pc(nc+1:$);nPc=nPc-nc;
    // (complex zeros real poles)
    nc=min(nZc,floor(nPr/2)) ;
    l=1
    for k=1:nc
        A=[Pr(l)+Pr(l+1) -Pr(l)*Pr(l+1);1 0];
        B=[1;0];
        C=[-2*real(Zc(k))+Pr(l)+Pr(l+1),abs(Zc(k))^2-Pr(l)*Pr(l+1)]
        l=l+2
        s=s*syslin(dt,A,B,C,1);
    end
    Zc=Zc(nc+1:$);nZc=nZc-nc;
    Pr=Pr(2*nc+1:$);nPr=nPr-2*nc;
    // (real zero, complex poles) pairs
    nrc=min(nZr,nPc)
    for k=1:nrc
        wn=abs(Pc(k));
        A=[2*real(Pc(k)) -wn;wn 0];
        B=[1;0];
        C=[1,-Zr(k)/wn];
        s=s*syslin(dt,A,B,C,0);
    end
    Zr=Zr(nrc+1:$);nZr=nZr-nrc;
    Pc=Pc(nrc+1:$);nPc=nPc-nrc;

    // remaining real poles
    for k=1:nPr
        s=s*syslin(dt,Pr(k),1,1,0);
    end
    //remaining complex poles
    for k=1:nPc
        wn=abs(Pc(k));
        A=[2*real(Pc(k)) -wn;wn 0];
        s=s*syslin(dt,A,[1;0],[0 1/wn],0);
    end
    //remaining real zeros (unproper system)
    if nZr>0 then
        if dt=="c" then var="s"; else var="z";end
        s=s*syslin(dt,[],[],[],poly(Zr,var,"r"));
    end
    //remaining complex zeros (unproper system)
    if nZc>0 then
        if dt=="c" then var="s"; else var="z";end
        s=s*syslin(dt,[],[],[],real(poly([Zc(:),conj(Zc(:))],var,"r")));
    end
    //apply gain
    s.C=s.C*K;
    s.D=s.D*K;
endfunction
