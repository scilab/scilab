
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function sol=chsolve(spcho,rhs)
    // Cholesky solver for A*sol=rhs (A is symmetric >0)
    // 1rst step: spcho=chfact(A)
    // 2nd step: sol=chsolve(spcho,rhs)
    // Example: N=20; A=sprand(N,N,0.1);
    // A=A*A'+speye(A);
    // sol=(1:N)'; rhs=A*sol;
    // spcho=chfact(A); sol=chsolve(spcho,rhs)
    perm=spcho("perm");
    sol=blkslv(spcho,rhs(perm));
    sol(perm)=sol
endfunction
