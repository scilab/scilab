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


function [a,u]=hermit(a)
    //[A,U]=hermit(A)
    //Hermite form: U is an unimodular matrix such that A*U is
    //triangular. The output value of A is A*U.
    //Warning: Experimental version
    //!
    if type(a)>2 then
        error(msprintf(gettext("%s: Wrong type for input argument: Polynomial array expected.\n"),"hermit"))
    end
    [m,n]=size(a);
    if m<>n then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n" ),"hermit",1))
    end
    [a,u]=htrianr(a)
    for l=n-1:-1:1
        dl(l:n)=degree(a(l,l:n));
        for k=l+1:n
            if dl(k)>=dl(l) then
                all=a(l,l);
                if norm(coeff(all),1) > 1.d-10 then
                    [r,q]=pdiv(a(l,k),a(l,l))
                    if l>1 then a(1:l-1,k)=a(1:l-1,k)-a(1:l-1,l)*q;end
                    a(l,k)=r
                    u(:,k)=u(:,k)-u(:,l)*q
                end
            end
        end
    end
endfunction
