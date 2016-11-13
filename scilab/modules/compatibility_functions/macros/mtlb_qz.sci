// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [AA, BB, Q, Z, V] = mtlb_qz(A,B)
    [lhs,rhs] = argn(0)
    [AA, BB, Q, Z] = schur(A,B)
    if lhs==5 then
        n  = size(A,1)
        LA = diag(AA)
        LB = diag(BB)
        V  = zeros(n,n)
        for k = 1:n
            K = kernel(A*LB(k)-B*LA(k))
            if size(K,2)<>1 then
                error(msprintf(gettext("%s: pencil is not diagonalizable.\n"),"mtlb_qz"))
            end
            V(:,k)=K
        end
    end
endfunction
