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

function [v]=ell1mag(eps,m1,z)
    //Function used for squared magnitude of an elliptic filter
    //Usually m1=eps*eps/(a*a-1);
    //  eps     :Passband ripple=1/(1+eps**2)
    //  m1      :Stopband ripple=1/(1+(eps**2)/m1)
    //  z       :Sample vector of values in the complex plane
    //  v       :Elliptic filter values at sample points
    //
    //!
    s=%sn(z,m1);un=ones(z);
    v=real(un./(un+eps*eps*s.*s))


endfunction
