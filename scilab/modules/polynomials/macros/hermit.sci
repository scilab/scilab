// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
