
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
