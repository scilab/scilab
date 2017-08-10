// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [sig,resid]=epred(r,s,q,coef,y,u,b0f)
    //<sig,resid>=epred(r,s,q,coef,y,u,b0f)
    //  Utilisee par armax1 pour calculer l'erreur de prediction
    //     coef= [-a1,..,-ar,b0,...,b_s,d1,...,d_q]'
    // ou    coef= [-a1,..,-ar,b1,...,b_s,d1,...,d_q]' si b0f=1
    //!

    [n1,n2]=size(y);
    t0=max(max(r,s+1),1)+1;
    if r<>0;XTM1=y((t0-1):-1:(t0-r));else XTM1=[];end
    if s<>-1;UTM1=u(t0-b0f:-1:(t0-s));else UTM1=[];end
    if q<>0;ETM1=0*ones(1,q);else ETM1=[];end
    npar=r+s+1-b0f+q
    ZTM1=[XTM1,UTM1,ETM1]';
    resid=0*ones(1,n2);
    for t=t0+1:n2,
        if r<>0;XT=[ y(t-1), XTM1(1:(r-1))];else XT=[];end
        if s<>-1;UT=[ u(t-b0f), UTM1(1:(s-b0f))];else UT=[];end
        resid(t)=y(t-1)- coef'*ZTM1;
        if q<>0;ET=[ resid(t), ETM1(1:(q-1))];else ET=[];end
        ZT=[XT,UT,ET]';
        XTM1=XT;UTM1=UT;ETM1=ET;ZTM1=ZT;
    end
    sig=1/(n2-t0)*sum(resid.*resid)
endfunction
