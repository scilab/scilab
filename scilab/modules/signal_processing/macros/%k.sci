// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.D
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [K]=%k(m)
    //K=%k(m)
    //Calculates Jacobi's complete elliptic integral
    //of the first kind:
    //  K = integral from 0 to 1 of
    //      [(1-t**2)(1-m*t**2)]**(-1/2)
    //m is allowed to be a vector
    //Ref :Abramowitz and Stegun page 598
    //  m :Parameter used in calculating the elliptic
    //    :integral where 0<m<1.
    //  K :Value of the elliptic integral from 0 to 1
    //    :on the real axis.
    //
    //!

    [n1,n2]=size(m);
    un=ones(n1,n2);
    a=un;
    b=sqrt(un-m);
    c=sqrt(m);
    while max(abs(c)) > %eps,
        an=0.5*(a+b);
        bn=sqrt(a.*b);
        cn=0.5*(a-b);
        a=an;
        b=bn;
        c=cn;
    end,
    K=%pi*un./(2*a);
endfunction
