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

function [Nz,Dz]=yulewalk(Norder, frq, mag)
    //YULEWALK  filter design using a least-squares method.
    //	Hz = YULEWALK(N,frq,mag) finds the N-th order iir filter
    //
    //		               n-1         n-2
    //		   B(z)   b(1)z     + b(2)z    + .... + b(n)
    //	     H(z)= ---- = ---------------------------------
    //		            n-1       n-2
    //		   A(z)    z   + a(1)z    + .... + a(n)
    //
    //which matches the magnitude frequency response given by vectors F and M.
    //Vectors frq and mag specify the frequency and magnitude of the desired
    //frequency response. The frequencies in frq must be between 0.0 and 1.0,
    //with 1.0 corresponding to half the sample rate. They must be in
    //increasing order and start with 0.0 and end with 1.0.
    //
    // Example: f=[0,0.4,0.4,0.6,0.6,1];H=[0,0,1,1,0,0];Hz=yulewalk(8,f,H);
    //fs=1000;fhz = f*fs/2;
    //clf(0);xset('window',0);plot2d(fhz',H');
    //xtitle('Desired Frequency Response')
    //[frq,repf]=repfreq(Hz,0:0.001:0.5);
    //clf(1);xset('window',1);plot2d(fs*frq',abs(repf'));
    //xtitle('Obtained Frequency Response')
    //
    [LHS,RHS]=argn(0);
    if RHS <>3
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"yulewalk",3));
    end
    npt=512;
    thelap=fix(npt/25);

    [mf,nf]=size(frq);
    [mm,nn]=size(mag);
    if mm ~= mf | nn ~= nf
        error(msprintf(gettext("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"),"yulewalk",2,3));
    end
    nbrk=max(mf,nf);
    if mf < nf
        frq=frq';
        mag=mag';
    end

    if abs(frq(1)) > %eps | abs(frq(nbrk) - 1) > %eps
        error(msprintf(gettext("%s: Wrong values for input argument #%d: The first element must be %s and the last %s.\n"),"yulewalk",2,"0","1"));
    end

    npt=npt+1;
    Ht=zeros(1,npt);

    nint=nbrk-1;
    df=frq(2:nf)-frq(1:nf-1);
    if (or(df < 0))
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be in increasing order.\n"),"yulewalk",2));
    end

    nb=1;
    Ht(1)=mag(1);
    for i=1:nint
        if df(i) == 0
            nb=nb - thelap/2;
            ne=nb + thelap;
        else
            ne=int(frq(i+1)*npt);
        end
        if (nb < 0 | ne > npt)
            error(msprintf(gettext("%s: Too abrupt change near end of frequency range.\n"),"yulewalk"));
        end
        j=nb:ne;
        if ne == nb
            inc=0;
        else
            inc=(j-nb)/(ne-nb);
        end
        Ht(nb:ne)=inc*mag(i+1) + (1 - inc)*mag(i);
        nb=ne + 1;
    end
    Ht=[Ht Ht(npt-1:-1:2)];
    n=max(size(Ht));
    n2=fix((n+1)/2);
    nb=Norder;
    nr=4*Norder;
    nt=0:1:nr-1;
    R=real(fft(Ht.*Ht,1));
    R =R(1:nr).*(0.54+0.46*cos(%pi*nt/(nr-1)));
    Rwindow=[1/2,ones(1,n2-1),zeros(1,n-n2)];

    nr=max(size(R));
    Rm=toeplitz(R(Norder+1:nr-1),R(Norder+1:-1:2));
    Rhs=- R(Norder+2:nr);
    denf=[1 Rhs/Rm'];

    A=polystab(denf);
    nA=size(A,"*");
    Dz=poly(A(nA:-1:1),"z","c");
    Qh=numf([R(1)/2,R(2:nr)],A,Norder);	// compute additive decomposition
    Qhsci=poly(Qh( size(Qh,"*"):-1:1 ),"z","c")
    aa=A(:);bb=Qh(:);
    nna=max(size(aa));nnb=max(size(bb));
    Ss=2*real((fft([Qh,zeros(1,n-nnb)],-1) ./ fft([A,zeros(1,n-nna)],-1)));
    hh=fft(exp(fft( Rwindow.*fft(log(Ss),1),-1)),1);
    impr=filter(1,A,[1 zeros(1,nr-1)]);
    B=real(hh(1:nr)/toeplitz(impr,[1 zeros(1,nb)])');
    nB=size(B,"*");
    Nz=poly(B(nB:-1:1),"z","c");
    B =real(numf(hh(1:nr),A,nb));
    if LHS==1 then
        Nz=syslin("d",Nz/Dz);
    end
endfunction

function b=polystab(a);
    //Utility function for use with yulewalk: polynomial stabilization.
    //	polystab(A), where A is a vector of polynomial coefficients,
    //	stabilizes the polynomial with respect to the unit circle;
    //	roots whose magnitudes are greater than one are reflected
    //	inside the unit circle.
    if length(a) == 1, b=a; return, end
    ll=size(a,"*");
    ap=poly(a($:-1:1),"s","coeff");
    v=roots(ap); i=find(v~=0);
    vs=0.5*(sign(abs(v(i))-1)+1);
    v(i)=(1-vs).*v(i) + vs./conj(v(i));
    b=a(1)*coeff(poly(v,"s"));
    b =b(ll:-1:1);
    if ~or(imag(a))
        b=real(b);
    end

endfunction

function b=numf(h,a,nb)
    //NUMF	Find numerator B given impulse-response h of B/A and denominator A
    //NB is the numerator order.  This function is used by yulewalk.

    nh=max(size(h));
    impr=filter(1,a,[1 zeros(1,nh-1)]);
    b=h/toeplitz(impr,[1 zeros(1,nb)])';
endfunction
