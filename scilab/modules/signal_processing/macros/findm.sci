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

function m=findm(chi)
    //Search for m such that chi = %k(1-m)/%k(m)
    //!

    if chi < 1 then
        t=1;
        tn=2;
        m=0.99999;
        mn=2;
        v=16*exp(-%pi/chi);
        while abs(t-tn) > 10.*%eps
            t=tn;
            lln=log(16/(1-m));
            k1=delip(1,sqrt(1-m));
            k=delip(1,sqrt(m));
            y=(k1*lln/%pi)-k;
            mn=m;
            m=1-v*exp((-%pi*y)/k1);
            tn=m+mn;
        end
    else
        t=1;
        tn=2;
        m=0.00001;
        mn=0.1;
        v=16*exp(-%pi*chi);
        while abs(t-tn) > 10.*%eps
            t=tn;
            lln=log(16/m);
            k1=delip(1,sqrt(1-m));
            k=delip(1,sqrt(m));
            y=(k*lln/%pi)-k1;
            mn=m;
            m=v*exp((-%pi*y)/k);
            tn=m+mn;
        end
    end
endfunction
