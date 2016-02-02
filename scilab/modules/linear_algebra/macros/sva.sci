
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

function [U,S,V]=sva(A,tol)
    // approximation de rang donne d'une matrice
    if A==[] then U=[],S=[],V=[],return,end
    [U,S,V]=svd(A,"e")
    if argn(2)==1 then
        tol = max(size(A)) * S(1) * %eps;
        rk = size(find(diag(S) > tol),"*");
    else
        if tol>1 then //rank given
            rk=tol
            if rk>min(size(A)) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: Requested rank is greater than matrix dimension."),"sva",1));
            end
        else
            rk = size(find(diag(S) > tol),"*");
        end
    end
    U=U(:,1:rk);S=S(1:rk,1:rk),V=V(:,1:rk)
endfunction



