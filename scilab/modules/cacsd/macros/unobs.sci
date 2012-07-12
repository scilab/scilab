// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarrre
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [dim,x]=unobs(A,C,tol)
// n first columns of x span the unobservable
// subspace of (A,C):
//          dim
//          [*,*]
// X'*A*X = [0,*]
//
//    C*X = [0,*]
// Copyright INRIA

[lhs,rhs]=argn(0);

if rhs < 2
    error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected."),"unobs", 2, 3));
end

if typeof(A) <> "constant" | ~isreal(A)
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A real matrix expected."),"unobs",1));
end

if typeof(C) <> "constant" | ~isreal(C)
    error(msprintf(gettext("%s: Wrong type for input argument #%d: A real matrix expected."),"unobs",2));
end

if rhs == 2
    tol=1.d-10*norm([A;C],1);
end

[p,p]=size(A);
[n,w]=contr(A',C',tol);
x=[w(:,n+1:p),w(:,1:n)];
dim=p-n;
endfunction
