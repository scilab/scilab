// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
