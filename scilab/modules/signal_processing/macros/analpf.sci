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

function [hs,pols,zers,gain]=analpf(n,fdesign,rp,omega)
    //[hs,pols,zers,gain]=analpf(n,fdesign,rp,omega)
    //Creates analog low-pass filter with cut-off frequency at omega
    //  n       :filter order (pos. integer)
    //  fdesign :filter design method
    //          :    fdesign=('butt','cheb1','cheb2','ellip')
    //  rp      :2-vector of error values for cheb1, cheb2, and
    //          :ellip filters where only rp(1) is used for
    //          :cheb1 case, only rp(2) is used for cheb2 case, and
    //          :rp(1) and rp(2) are both used for ellip case.
    //          :          0<rp(1),rp(2)<1
    //          :for cheb1 filters:  1-rp(1)<ripple<1 in passband
    //          :for cheb2 filters:  0<ripple<rp(2)   in stopband
    //          :for ellip filters:  1-rp(1)<ripple<1 in passband
    //          :                    0<ripple<rp(2)   in stopband
    //  omega   :cut-off frequency of low-pass filter in rd/s
    //  hs      :rational polynomial transfer function
    //  pols    :poles of transfer function
    //  zers    :zeros of transfer function
    //  gain    :gain of transfer function
    //
    //    hs=gain*poly(zers,'s')/poly(pols,'s')
    //
    //!


    select fdesign
    case "butt" then
        [pols,gain]=zpbutt(n,omega);
        zers=[];
        hs=gain/real(poly(pols,"s"));
    case "cheb1" then
        epsilon=sqrt(-1+1/(1-rp(1))**2);
        [pols,gain]=zpch1(n,epsilon,omega);
        zers=[];
        hs=gain/real(poly(pols,"s"));
    case "cheb2" then
        att=1/rp(2);
        [zers,pols,gain]=zpch2(n,att,omega);
        hs=gain*real(poly(zers,"s"))./real(poly(pols,"s"));
    case "ellip" then
        epsilon=sqrt(-1+1/(1-rp(1))**2);
        att=1/rp(2);
        m=find_freq(epsilon,att,n);
        omegar=omega/sqrt(m);
        [zers,pols,gain]=zpell(epsilon,att,omega,omegar);
        hs=gain*real(poly(zers,"s"))./real(poly(pols,"s"));
    else
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
        "analpf",2,"''butt'',''cheb1'',''cheb2'',''ellip''"));
    end
    hs.dt="c";
endfunction
