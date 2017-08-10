// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
// Copyright (C) INRIA - 1996 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [zers,pols,gain]=zpch2(n,A,omegar)
    //[zers,pols,gain]=zpch2(n,A,omegar)
    //Poles and zeros of a type 2 Chebyshev analog filter
    //gain is the gain of the filter
    //H(s)=gain*poly(zers,'s')/poly(pols,'s')
    //  n      :Filter order
    //  A      :Attenuation in stop band (A>1)
    //  omegar :Cut-off frequency in rd/s
    //  zers   :Resulting filter zeros
    //  pols   :Resulting filter poles
    //  gain   :Resulting filter gain
    //
    //!
    un=ones(1,n);
    v=%pi/(2*n)*(1:2:2*n-1);
    w=exp(%i*v);
    cosine=real(w);
    sine=imag(w);
    n2=int(n/2);
    if n==2*n2 then,
        zers=%i*omegar*un./cosine;
    else,
        zers=%i*omegar*un(1:n-1)./[cosine(1:n2),cosine(n2+2:n)];
    end,
    Gamma=(A+sqrt(A*A-1))**(1/n);
    alpha=-((Gamma-1/Gamma)/2)*sine;
    Beta=((Gamma+1/Gamma)/2)*cosine;
    normal=alpha.*alpha+Beta.*Beta;
    pols=omegar*(alpha-%i*Beta)./normal;
    gain=abs(real(prod(pols)/prod(zers)));

endfunction
