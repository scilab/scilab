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

function [hz,zz,gz]=iir(n,ftype,fdesign,frq,delta)
    // hz=iir(n,ftype,fdesign,frq,delta)
    //macro which designs an iir digital filter
    //using analog filter designs.
    //the arguments of the filter are:
    //  n        :filter order (pos. integer)
    //  ftype    :specification of the filter type
    //           :   ftype=('lp','hp','bp','sb')
    //  fdesign  :specifiation of the analog filter design
    //           :   fdesign=('butt','cheb1','cheb2','ellip')
    //  frq      :2-vector of discrete cut-off frequencies
    //           :(i.e., 0<frq<.5). For lp and hp filters only
    //           :frq(1) is used.  For bp and sb filters frq(1)
    //           :is the lower cut-off frequency and frq(2) is
    //           :the upper cut-off frequency
    //  delta    :2-vector of error values for cheb1, cheb2, and
    //           :ellip filters where only delta(1) is used for
    //           :cheb1 case, only delta(2) is used for cheb2 case,
    //           :and delta(1) and delta(2) are both used for
    //           :ellip case.
    //           :          0<delta(1),delta(2)<1
    //           :for cheb1 filters:  1-delta(1)<ripple<1 in passband
    //           :for cheb2 filters:  0<ripple<delta(2)   in stopband
    //           :for ellip filters:  1-delta(1)<ripple<1 in passband
    //           :                    0<ripple<delta(2)   in stopband
    //
    //!
    if and(argn(1)<>[1 3]) then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d or %d expected.\n"),"iir",1,3))
    end
    //select analog filter design for low-pass filter with fc=.25
    if type(n)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"iir",1))
    end
    if  size(n,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),"iir",1))
    end
    if  n<0|n<>round(n) then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"),"iir",1))
    end

    if and(ftype<>["lp","hp","bp","sb"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"iir",2,"''lp'',''hp'',''bp'',''sb''"))
    end
    if and(fdesign<>["butt","cheb1","cheb2","ellip"]) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"iir",2,"''butt'',''cheb1'',''cheb2'',''ellip''"))
    end
    if type(frq)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"iir",4))
    end
    if type(delta)<>1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"iir",5))
    end

    if max(abs(frq))>0.5 then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be in the interval [%s, %s].\n"),"iir",4,"0","0.5"));
    end
    if delta(1)<0|delta(2)>1 then
        error(msprintf(gettext("%s: Wrong values for input argument #%d: Elements must be in the interval [%s, %s].\n"),"iir",4,"0","1"));
    end

    [hs,pc,zc,gc]=analpf(n,fdesign,delta,2);
    //make digital low-pass filter from analog low-pass filter
    z=poly(0,"z");[pd,zd,gd]=bilt(pc,zc,gc,2*(z-1),(z+1));
    //do change of variables to obtain general digital filter
    if argn(1)==1 then
        hz=trans(pd,zd,gd,ftype,frq);
    else
        [pz,zz,gz]=trans(pd,zd,gd,ftype,frq);
        hz=pz
    end
endfunction
