// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2013 - Samuel GOUGEON : restriction to decimal numbers removed
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function y = flipdim(x, dim, sb)

    // FLIPDIM function
    // Given x, a scalar/vector/matrix of any type, two integers dim and sb, this function flips the x components by blocks along the dimension number dim (x and y have the same size).
    // -Inputs :
    //  x : a scalar/vector/array
    //  dim : a positive integer
    //  sb : size of the block to permute
    // -Output :
    //  y : a scalar/vector/array
    //
    // F.Belahcene

    rhs = argn(2);
    if rhs < 2 then
        msg = _("%s: Wrong number of input argument(s): %d to %d expected.\n");
        error(msprintf(msg, "flipdim", 2, 3));
    end

    if size(dim, "*") <> 1 then
        msg = _("%s: Wrong size for input argument #%d: A positive integer expected.\n")
        error(msprintf(msg, "flipdim", 2));
    elseif type(dim) <> 8 & (type(dim) <> 1 | dim < 1 ) then
        msg = _("%s: Wrong type for input argument #%d: A positive integer expected.\n");
        error(msprintf(msg, "flipdim", 2));
    end
    if rhs >= 3 then
        if size(sb, "*") <> 1 then
            msg = _("%s: Wrong size for input argument #%d: A positive integer expected.\n")
            error(msprintf(msg, "flipdim", 3));
        elseif type(sb) <> 8 & (type(sb) <> 1 | sb < 1 ) then
            msg = _("%s: Wrong type for input argument #%d: A positive integer expected.\n");
            error(msprintf(msg, "flipdim", 3));
        elseif dim > 2 then
            msg = _("%s: Cannot flip hypermatrix blockwise. %d input arguments expected.\n");
            error(msprintf(msg, "flipdim", 2));
        end
    else
        sb = 1;
    end

    dim = floor(dim);
    sb  = floor(sb);

    if dim > ndims(x)
        y = x;
        return
    elseif or(dim == [1 2]) & ndims(x) < 3 then  // flipdim(x, dim, sb) is not suited for hypermatrices.

        if dim == 1 then
            x = x.';
        end

        nC = size(x, 2);
        if modulo(nC, sb) ~= 0 then
            msg = _("%s: Wrong value for input argument #%d: A divisor of the selected dimension size expected.\n");
            error(msprintf(msg, "flipdim", 3));
        end
        nb = nC/sb; // Number of blocks.
        c2 = ((nb:-1:1).*.ones(1,sb))*sb + ones(1, nb).*.(1-sb:0);
        y = x(:, c2);
        if dim == 1 then
            y = y.';
        end

    else

        l = list();
        for k = 1:dim - 1
            l(k) = eye();
        end
        l(dim) = $:-1:1;
        for k = dim + 1:ndims(x)
            l(k) = eye();
        end

        y = x(l(:));

    end

endfunction
