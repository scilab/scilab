// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [num,den]=frfit(w,fresp,order,weight)
    //Syntax:
    //[num,den]=frfit(w,fresp,r,weight)
    //sys=frfit(w,fresp,r,weight)
    //
    //  w: vector of frequencies in Hz
    //  fresp: vector of frequency responses at these frequencies.
    //  weight: vector of weights given to each point
    //
    //  Fits frequency response data points by a bi-stable transfer
    //  function
    //                G(s) = num(s)/den(s)
    //  of order r.
    //  freq(num,den,%i*w) should be close to fresp
    //
    // changing frequencies to rad/s
    w=2*%pi*w;
    [LHS,RHS]=argn(0);
    if RHS==3
        weight=ones(w);
    end
    w=w(:);fresp=fresp(:);weight=weight(:);

    Mean=sum(fresp)/size(fresp,"*");
    if max(abs(fresp-Mean)) < .1*max(abs(fresp))
        num=real(Mean); den=1; return
    end

    order1=order+1;npts=length(w);wmed=1;

    if RHS < 4 then
        weight=ones(npts,1);
    end

    M0=zeros(order+1,1); M1=M0;
    M0(1)=1; M1(2)=1; M=[M0 M1];
    for i=1:order-1,
        Mt=2*[0;M1(1:order)]-M0;
        M=[M Mt];
        M0=M1; M1=Mt;
    end

    k=2;km=1;
    sl0=round((log10(abs(fresp(k)))-log10(abs(fresp(km))))/(log10(w(k))-log10(w(km))));
    k=npts;km=k-1;
    slinf=round((log10(abs(fresp(k)))-log10(abs(fresp(km))))/(log10(w(k))-log10(w(km))));

    if slinf>0&slinf<20
        w=[w;[10;15]*w(npts)];
        fresp=[fresp;[1;1]*10.^slinf*abs(fresp(npts))];
        weight=[weight;1;1];
        npts=npts+2;
        slinf=0;
    end


    if sl0>0
        mindeg=max(abs(sl0),-slinf+abs(sl0));
    else
        mindeg=max(abs(sl0),-slinf);
    end

    if mindeg > order
        warning(msprintf(gettext("%s: Filter order too small.\n"),"frfit"));
        sl0=sign(sl0)*min(order,sl0);
        if sl0>0 then
            slinf=-(order-abs(sl0));
        else
        end
        slinf=-order;
    end


    jw=%i*w;mag=abs(fresp);t0=ones(npts,1); t1=jw;A=[ones(npts,1),jw];

    for i=1:order-1,
        t=2*jw.*t1-t0;
        A=[A,t];
        t0=t1; t1=t;
    end

    Aom=A;

    //A=[A -diag(fresp)*A];AA=A;om0=w;
    A=[A, -fresp*ones(1,size(A,2)).*A];AA=A;om0=w;

    Acons=[]; ycons=[];
    if sl0<=0
        Acons=[Acons; zeros(-sl0,order1) M(1:-sl0,:);...
        M(1,:) -(mag(1)*w(1)^(-sl0))*M(1-sl0,:)];
        ycons=[ycons; zeros(-sl0+1,1)];
    elseif sl0>0
        Acons=[Acons; M(1:sl0,:) zeros(sl0,order1);...
        M(1+sl0,:)*w(1)^sl0 -mag(1)*M(1,:)];
        ycons=[ycons; zeros(sl0+1,1)];
    end

    Acons=[Acons;zeros(1,order1) M(order1,:)];ycons=[ycons;1];

    if slinf<=0
        Acons=[Acons; M(order1+slinf:order1,:) zeros(-slinf+1,order1)];
        ycons=[ycons; mag(npts)*w(npts)^(-slinf); zeros(-slinf,1)];
    end

    [nc,nv]=size(Acons);

    if nc >= nv
        x=Acons\ycons;
    else
        indin=1:nv; indout=[];
        for i=1:nc,
            if i<nc,
                [m,ix]=max(abs(Acons(i:nc,i:nv)));ix=ix(2);
            else
                [m,ix]=max(abs(Acons(i:nc,i:nv)));
            end
            Acons(:,i:nv)=[Acons(:,i+ix-1) Acons(:,[i:i+ix-2,i+ix:nv])];
            indout=[indout,indin(ix)];indin=indin([1:ix-1,ix+1:nv-i+1]);

            x=Acons(i:nc,i); Atmp=Acons(i:nc,i+1:nv); ytmp=ycons(i:nc);
            if x(1)>=0
                sgn=1;
            else
                sgn=-1;
            end
            aux=sgn*sqrt(x'*x);x(1)=x(1)+aux;nx2=.5*x'*x;
            Atmp=[[-aux;zeros(nc-i,1)] Atmp-x*((x'*Atmp)/nx2)];
            ytmp=ytmp-x*((x'*ytmp)/nx2);
            Acons(i:nc,i:nv)=Atmp;ycons(i:nc)=ytmp;
        end
        perm=[indout indin];
        Ac1=Acons(:,1:nc); Ac2=Acons(:,nc+1:nv);
        A=A(1:npts,:);
        A=[real(A);imag(A)];
        A=A(:,perm);
        A1=A(:,1:nc); A2=A(:,nc+1:nv);
        A=A2-A1*(Ac1\Ac2);
        y=-A1*(Ac1\ycons);

        fweight=ones(npts,1);
        ind=find(w > 10);

        fweight(ind)=((1) ./(w(ind).^order));
        ind=find(w < .01);

        fweight(ind)=(1./(w(ind)^min(0,sl0)))';
        fweight=weight.*fweight;
        //Wt=diag([fweight;fweight]);
        //x=pinv(Wt*A)*(Wt*y);
        //x=pinv([fweight;fweight]*ones(1,size(A,2)).*A)*(vvv.*y);
        vvv=[fweight;fweight];
        x=(vvv*ones(1,size(A,2)).*A)\(vvv.*y);
        x=[Ac1\(ycons-Ac2*x);x];
        [s,perm]=gsort(-perm);
        s=-s;
        x=x(perm);

        nresp=Aom*x(1:order1); dresp=Aom*x(order1+1:2*order1);
        relerr=abs(abs(nresp./dresp)./mag-1)';
        for kk=1:prod(size(relerr));relerr(kk)=min(1,relerr(kk));end
        relerr=relerr(:);
        ind=find(mag < .01); relerr(ind)=relerr(ind)*.3;
        ind=find(relerr>.5);
        if ind<>[]
            fweight(ind)=( exp(relerr(ind)*log(10)) ).*fweight(ind);
        end
        //Wt=diag([fweight;fweight]);
        //x=pinv(Wt*A)*(Wt*y);
        //x=pinv([fweight;fweight]*ones(1,size(A,2)).*A)*(vvv.*y);
        x=([fweight;fweight]*ones(1,size(A,2)).*A)\(vvv.*y);
        x=[Ac1\(ycons-Ac2*x);x];x=x(perm);
    end

    a=x(1:order1); b=x(order1+1:2*order1);
    num=fliplr((M*a)');den=fliplr((M*b)');

    err=abs(AA*x);
    num=num(-slinf+1:order1);

    junk=poly(fliplr(num),"s","c")
    rn=roots(junk); rn=-abs(real(rn))+%i*imag(rn);
    l=length(rn);
    for k=1:l
        if real(rn(k))>-1e-3
            rn(k)=-1.e-3+%i*imag(rn(k))
        end
    end

    junk=poly(fliplr(den),"s","c");
    dn=roots(junk); dn=-abs(real(dn))+%i*imag(dn);
    l=length(dn);
    for k=1:l
        if real(dn(k))>-1e-3
            dn(k)=-1.e-3+%i*imag(dn(k))
        end
    end

    polyrn=poly(fliplr(rn),"s");
    polydn=poly(fliplr(dn),"s");
    num=real(num(1)*polyrn);
    den=real(den(1)*polydn);
    if LHS==1
        num=syslin("c",num/den)
    end

endfunction
function m=fliplr(m)
    //Utility fct
    [p,q]=size(m);
    m=m(:,q:-1:1);
endfunction
