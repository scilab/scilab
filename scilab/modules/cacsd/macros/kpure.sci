// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [K,R]=kpure(sl,eps)
    //if sl is a transfert function N(S)/D(s) kpure looks for K producing
    //pure  imaginary roots for
    //  P(s)=D(s)+K*N(s)
    //There is a pair of pure imaginary poles if and only if
    //  P(%i*q)=0  (1)
    // and
    //  P(-%i*q)=0 (2)
    // because N and D are polynomials with real coefficients.

    //Author: Serge Steer, INRIA
    y=[];R=[];
    msg=_("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n")
    if argn(2)==1 then eps=1e-6,end
    if size(eps,"*")==2 then  eps=eps(2),end //compatibility
    select typeof(sl)
    case "rational" then
        if size(sl.num,"*") <> 1 then
            error(msprintf(msg,"kpure",1))
        end
    case "state-space" then
        if size(sl.D,"*") <> 1 then
            error(msprintf(msg,"kpure",1))
        end
        sl=ss2tf(sl)
    else
        error(msprintf(msg,"kpure",1))
    end

    //(1) give K(s)=-D(s)/N(s)
    s=poly(0,varn(sl.den))
    K=-sl.den/sl.num;
    // replace K by the previous value in (2) and find the roots
    s=roots(numer(horner(sl.den,-s)+K*horner(sl.num,-s)),"e");
    //retain pure imaginary roots
    s=imag(s(abs(real(s))<eps));
    R=(s(s>0).'*%i);
    //find the K(s) values K(s)=-D(s)/N(s)
    K=-real(freq(sl.den,sl.num,R))
endfunction
