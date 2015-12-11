// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function a=%sp_i_sp(i,j,b,a)

    [lhs,rhs]=argn(0)
    if rhs==3 then
        a=b;
        b=j;
        [m,n]=size(a)
        a=a(:)
        a(i)=b(:)
        a=matrix(a,m,n)
    elseif rhs == 4 then
        // bruno (feb 20 2006) add the following cases not taken
        // into account by the interface spops :
        if size(b,1) == 1 & size(b,2) == 1 then
            //   A(i,j) = B with B a 1x1 sparse matrix
            a(i,j) = full(b)
        elseif i == : & j == : then
            //   A(:,:) = B with mA x nA = mB x nB but mA ~= mB
            //   B is reshaped with the sizes of A
            [m,n] = size(a)
            a = matrix(b,m,n)
        else
            // print an error
            error(msprintf(_("%s: Sparse insertion case is not implemented.\n"),"%sp_i_sp"));
        end
    end
endfunction
