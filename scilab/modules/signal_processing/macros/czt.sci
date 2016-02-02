// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [czx]=czt(x,m,w,phi,a,theta)
    //czx=czt(x,m,w,phi,a,theta)
    //chirp z-transform algorithm which calcultes the
    //z-transform on a spiral in the z-plane at the points
    //[a*exp(j*theta)][w**kexp(j*k*phi)] for k=0,1,...,m-1.
    //  x     :Input data sequence
    //  m     :czt is evaluated at m points in z-plane
    //  w     :Magnitude multiplier
    //  phi   :Phase increment
    //  a     :Initial magnitude
    //  theta :Initial phase
    //  czx   :Chirp z-transform output
    //!

    //get the size of x and find the maximum of (n,m)

    n=max(size(x));
    nm=max([n,m]);

    //create sequence h(n)=[w*exp(-j*phi)]**(-n*n/2)

    w=w*exp(-%i*phi);
    idx=(-nm+1:0);
    idx=-idx.*idx/2;
    h=exp(idx*log(w));
    h(nm+1:2*nm-1)=h(nm-1:-1:1);

    //create g(n)

    a=a*exp(%i*theta);
    idx=(0:n-1);
    g=exp(-idx*log(a))./h(nm:nm+n-1);
    g=x.*g;

    //convolve h(n) and g(n)

    hc=h(nm:nm+m-1);
    hc(m+1:m+n-1)=h(nm-n+1:nm-1);
    gc=0*ones(hc);
    gc(1:n)=g;
    hf=fft(hc,-1);
    gf=fft(gc,-1);
    hcg=fft(hf.*gf,1);

    //preserve m points and divide by h(n)

    czx=hcg(1:m)./h(nm:nm+m-1);
endfunction
