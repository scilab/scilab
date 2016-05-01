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
