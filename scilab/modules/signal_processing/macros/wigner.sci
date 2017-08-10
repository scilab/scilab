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

function tab=wigner(x,h,deltat,zp)
    //macro which computes the 'time-frequency' wigner
    //spectrum of a signal.
    //
    //	tab    : wigner spectrum (lines correspond to the time variable)
    //	x      : analysed signal
    //	h      : data window
    //      deltat : analysis time increment (in samples)
    //      zp     : length of FFT's. %pi/zp gives the frequency increment.
    //!
    //
    //   Initializations
    //
    l=prod(size(x));
    n=prod(size(h));
    npr=2*n;
    h=h.*conj(h);tab=[];
    //
    //   Analytical signal computation using Hilbert transform
    //
    [y,y1]=convol(hilb(127),x);
    z=x+%i*y;
    //
    //   Wigner distribution computation
    //
    t=n;
    while t<=l-n,
        z1=h.*z(t:t+n-1).*conj(z(t:-1:t-n+1));
        z1(zp)=0;
        w=fft(z1,-1);
        w=2*(2*real(w)-z(t)*z(t)'*ones(w));tab=[tab;w];
        t=t+deltat;
    end;
    tab=tab(:,1:zp);
endfunction
