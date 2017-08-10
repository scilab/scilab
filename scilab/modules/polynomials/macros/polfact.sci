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


function [f]=polfact(p)
    // Minmal factors of p
    // f=polfact(p)
    //
    // p : polynomial
    // f : vector [f0 f1 ... fn] such that p=prod(f)
    //     - f0  constant
    //     - fi polynomial
    //!
    //
    if type(p)>2 then
        error(msprintf(gettext("%s: Wrong type for input argument: Polynomial array expected.\n"),"polfact"))
    end
    if size(p,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A polynomial expected.\n"),"polfact",1))
    end

    if norm(imag(coeff(p)))<>0 then
        error(msprintf(gettext("%s: Input argument #%d must be real.\n"),"polfact",1))
    end
    p=real(p)
    n=degree(p);f=coeff(p,n);
    if n==0 then return,end
    var=varn(p);
    r=roots(p);[s,k]=gsort(abs(r));r=r(k)
    k=1;
    while k<=n do,
        if imag(r(k))<>0 then
            f=[f,real(poly(r(k:k+1),var))]
            k=k+2
        else
            f=[f,poly(r(k),var)]
            k=k+1
        end
    end
endfunction
