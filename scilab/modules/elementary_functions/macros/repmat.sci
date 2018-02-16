// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2011 -  INRIA, Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) - 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function B = repmat(A,varargin)

    // CHECKING INPUT ARGUMENTS
    // ------------------------
    rhs = argn(2);
    if rhs < 2 then
        msg = _("%s: Wrong number of input arguments: at least %d expected.\n")
        error(msprintf(msg, "repmat", 2))
    end

    if A == [] then
        B = A
        return
    end

    narg = size(varargin);

    // Reading & setting replication numbers
    if narg == 1 then
        // Scalar of vector needed
        v = varargin(1);
        if type(v) <> 1 | ~isreal(v, 0) | ~and(v == int(v)) | v == [] | v <0 then
            msg = _("%s: Argument #%d: Non-negative integers expected.\n")
            error(msprintf(msg, "repmat", 2))
        else
            v = real(v);    // in case of complex encoding with null imag
        end
        if ~isscalar(v) & ~isvector(v) then
            msg = _("%s: Wrong size for input argument #%d: A scalar or vector expected.\n")
            error(msprintf(msg, "repmat", 2))
        end
        sizes = v;
        if length(v) == 1 then
            sizes = [v,v];
        end
    else
        sizes = [];
        for i = 1:narg
            v = varargin(i);
            if type(v) <> 1 | ~isreal(v, 0) | ~and(v == int(v)) | v == [] | v < 0 then
                msg = _("%s: Argument #%d: Non-negative integers expected.\n")
                error(msprintf(msg, "repmat", i+1))
            end
            if length(v)<>1 then
                msg = _("%s: Argument #%d: Scalar (1 element) expected.\n")
                error(msprintf(msg, "repmat", i+1))
            end
            sizes = [sizes v];
        end
    end

    // Preprocessing sizes = replication factors
    // -----------------------------------------
    // Trimming ending ones in sizes
    for s = length(sizes):-1:3
        if sizes(s) == 1 then
            sizes(s) = []
        else
            break
        end
    end
    // B expected sizes
    sA = size(A)
    ndimsA = length(sA)
    ndimsR = length(sizes)
    if ndimsA >= ndimsR then
        Bsizes = size(A).*[sizes ones(1,(ndimsA-ndimsR))]
    else
        Bsizes = [sA ones(1,(ndimsR-ndimsA))] .* sizes
    end
    // Sparse matrices are only 2D (limited output dimensions):
    if or(type(A) == [5 6]) & length(Bsize) > 2 then
        msg = _("%s: Wrong dimensions for input arguments: The sparse result must be 2D, not %dD.\n");
        error(msprintf(msg, "repmat", length(Bsizes)));
    end
    // Replication factors
    Rfactors = list();
    for s = sizes
        Rfactors($+1) = s
    end

    // PROCESSING
    // ----------
    if find(sizes == 0) then
        B = []
        return
    end

    // Special case where the input A is scalar: special efficient processing
    if size(A,"*") == 1 then
        i = uint8(0);
        i(Rfactors(:)) = 0;
        i = i + 1;
        if typeof(A) <> "rational" then
            B = matrix(A(i), sizes);
        else
            B = matrix(A(i,0), sizes);
        end
        return
    end

    // Numerical data: double, integers, polynomials, sparse : kron() is used
    if or(type(A) == [1 2 5 7 8]) then
        B = ones(Rfactors(:)) .*. A;
        return
    end

    // Other regular types : booleans, textes..:
    // => we replicate and use the matrix of linearized indices of A components
    if or(type(A) == [4 6 9 10]) then
        // Processing (without intermediate variable, to save memory)
        B = matrix(A(ones(Rfactors(:)) .*. matrix(1:size(A,"*"), size(A))), Bsizes);
        return
    end

    // Rationals, other mlists, overloads
    if typeof(A) == "rational" then
        B = rlist(repmat(A.num,sizes), repmat(A.den,sizes), A.dt)
    else
        execstr("B=%" + typeof(A) + "_repmat(A, sizes)")
    end
endfunction
