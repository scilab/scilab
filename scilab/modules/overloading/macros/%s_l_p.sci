// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function f=%s_l_p(m,p)
    // m\p with p matrix of polynomials m matrix of scalar
    //!

    [l,c]=size(m)
    [mp,np]=size(p);
    if l==c then
        f=inv(m)*p
    else
        s=poly(0,varn(p))
        f=m\coeff(p,0)
        for k=1:max(degree(p))
            f=f+(m\coeff(p,k))*(s^k)
        end
    end
endfunction
