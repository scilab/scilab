// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// THIS OVERLOAD SEEMS NO LONGER CALLED  => TO BE DELETED

function [x,k] = %hm_max(varargin)
    n = size(varargin)
    v = varargin(1)

    // One input argument
    if n == 1 then
        // Retrieve the dimensions
        m = size(v)
        // Compute the max of column vector
        [x,k] = max(v(:))
        // check if lhs = 2 and if varargin(1) is vector
        if (argn(1) > 1 & (m($) <> size(v, "*"))) then    // not sure this condition is enough..
            k = ind2sub(m, k(1))
        end
    else
        if n==2 then
            d = varargin(2)
            // Check if d is a string
            if type(d)==10 then
                [x,k] = %hm_oriented_max(v,d)
                return
            end
        end
        // if the second input argument is a matrix or hypermatrix
        // Obtain column vector
        x = v(:)
        dims = size(v)
        for kk = 2:n
            sz = size(varargin(kk))
            // check if the dims are different
            if or(dims<>sz) then
                if prod(dims)<>1 & prod(sz)<>1 then
                    msg = _("%s: Arguments #%d and #%d must have compatible sizes\n")
                    error(msprintf(msg, "%hm_max",1, kk))
                end
                // the first argument is a scalar
                if prod(dims)==1 then
                    dims = sz
                end
            end
            // max between hypermatrix and hypermatrix
            [x,k] = max(x,varargin(kk)(:))
        end
        x = matrix(x, dims)
        k = matrix(k, dims)
    end
endfunction

function [x,k] = %hm_oriented_max(m,d)
    dims = size(m)
    if d=="m" then
        d = find(dims>1,1)
        if d==[] then
            [x,k1] = max(m(:))
            k = ind2sub(dims, k1(1))
            return
        end
    elseif d=="r" then
        d = 1
    elseif d=="c" then
        d = 2
    end

    N   = size(dims,"*");
    p1  = prod(dims(1:d-1));// step for one max
    p2  = p1*dims(d);       //step for beginning of vector to max
    ind = (0:p1:p2-1)';     // selection for vector to max
    deb = (1:p1);
    I   = ind*ones(deb) + ones(ind)*deb

    ind = (0:p2:prod(dims)-1);
    I = ones(ind).*.I + ind.*.ones(I)

    [x,k] = max(matrix(m(I),dims(d),-1),"r")

    dims(d) = 1
    if d==N then
        dims = dims(1:$)
    else
        dims(d) = 1
    end

    x = matrix(x, dims)
    k = matrix(k, dims)
endfunction










