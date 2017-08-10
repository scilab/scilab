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

function f=%p_r_s(p,m)
    // f=%p_r_s(p,m) <=> f=p/m
    // with p matrix of polynomials, m constant matrix
    //!

    [l,c]=size(m)
    [mp,np]=size(p);
    if l==c then
        f=p*inv(m)
    else
        s=poly(0,varn(p))
        f=coeff(p,0)/m
        for k=1:max(degree(p))
            f=f+(coeff(p,k)/m)*(s^k)
        end
    end
endfunction
