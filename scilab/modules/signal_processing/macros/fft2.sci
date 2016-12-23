
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F.B
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
function x = fft2(varargin)
    // Two-dimensional fast Fourier transform
    // Syntax : y = fft2(x) or y = fft2(x,m,n)
    // Inputs :
    // x : scalar, vector, matrix, array (real or complex)
    // m, n : numbers (respectively) of rows and colums of x which used for the perfom of DFT,if the rows number respectively (columns) of x is more than m then x is truncated in order to have m rows, else if the rows (respectively columns) number of x is less than m then x rows are completed by 0 to have m rows.
    //
    // Outputs :
    // y : scalar, vector, matrix, array (real or complex), if there is one input argument x then y and x have the same size, else if there are 3 inputs arguments then the sizes of the first and second dimension of y are equal to m and n, the others dimension sizes are equal to the size of x

    if ~or(argn(2)==[1 3])
        msg = _("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, "fft2", 1, 3))
    end
    if type(varargin(1))~=1
        msg = _("%s: Argument #%d: Decimal or complex number(s) expected.\n")
        error(msprintf(msg, "fft2", 1))
    end
    if size(varargin) == 1 then
        a = varargin(1);
        if ndims(a)<3 then
            x = fft(a);
        else
            dims = size(a)
            v = a(:)
            incr = 1;
            for k = 1:2
                dk = dims(k)
                v = fft(v ,-1,dk,incr);
                incr = incr*dk;
            end
            x = matrix(v, dims)
        end

    elseif size(varargin) == 3 then
        a = varargin(1);
        m = varargin(2);
        n = varargin(3);
        asize1 = size(a,1);
        asize2 = size(a,2);
        if ndims(a)<3 then
            x(1:min(m,asize1),1:asize2) = a(1:min(m,asize1),1:asize2);
        else
            dims = size(a)
            dims([1 2]) = [m n]
            x = resize_matrix(0, dims)
            for i = 1:prod(dims(3:$))
                x(1:min(m,asize1),1:min(n,asize2),i) = a(1:min(m,asize1),1:min(n,asize2),i);
            end
        end
        x = fft2(x)
    end
endfunction
