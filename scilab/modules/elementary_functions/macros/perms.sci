// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = perms(x)

    // PERMS function
    // Given a vector x (size: 1*n or n*1), this function returns all the permutations of x (i.e n! permutations)
    // Inputs :
    //  x : scalars/vectors
    // output :
    // y : matrix
    // F.Belahcene

    [lhs, rhs] = argn(0);
    if rhs <> 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"perms", 1));
    end

    if size(x,"*")==1 | size(x,"*")==0 then
        y = x;
    elseif size(x,1)<>1 & size(x,2)<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: Vector expected.\n"),"perms"));
    else
        xsize = size(x, "*");
        indrec = perms(1:xsize-1);
        ind = [ones(prod(1:xsize-1),1)*xsize, indrec];
        for j = xsize-1:-1:1
            indrec(indrec==j) = j + 1;
            ind = [ind; [ones(prod(1:xsize-1),1)*j,indrec]];
        end
        y = matrix(x(ind), size(ind));
    end

endfunction

