// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [lnum,lden,g]=factors(P,flag)
    //Given a polynomial or rational P, returns in list lnum polynomials of
    //degree 1 or two which are the factors of numerators of P.
    // and in lden the factors of denominator of P. g is the gain.
    // if flag=='c' unstable roots are reflected vs the imaginary axis
    // if flag=='d' unstable roots are reflected vs unit circle
    [LHS,RHS]=argn(0);
    if RHS==1 then flag=[];end
    select typeof(P)
    case "polynomial" then
        if size(P,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A polynomial expected.\n"),"factors",1))
        end
        [lnum,lden]=pfactors(P,flag);
    case "rational" then
        if size(P,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A rational fraction expected.\n"),"factors",1))
        end
        [lnum,gn]=pfactors(P.num,flag);
        [lden,gd]=pfactors(P.den,flag);
        g=gn/gd;
        if LHS==1 then
            num=g;
            for k=lnum;num=num.*k;end
            den=1;
            for k=lden;den=den.*k;end
            lnum=syslin(P.dt,num,den);return
        end
    case "state-space" then
        if size(P,"*")<>1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A single input, single output system expected.\n"),"factors",1))
        end

        P=ss2tf(P)
        [lnum,gn]=pfactors(P.num,flag);
        [lden,gd]=pfactors(P.den,flag);g=gn/gd;
        if LHS==1 then
            num=g;for k=lnum;num=num.*k;end
            den=1;for k=lden;den=den.*k;end
            lnum=syslin(P.dt,num,den);return
        end
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear dynamical system or polynomial array expected.\n" ),"factors",1))
    end
endfunction
