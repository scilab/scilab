// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrifht (C) 2012 - Scilab Enterprises - Cedric Delamarre
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2016 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [S, k] = %hm_gsort(A, method, varargin)
    // method can be: 'g', 'r', 'c', 1, 2, .., ndims, 'lr', 'lc'
    // sortdir = varargin(1): "i", "d", or vector of "i" and "d"
    // criteria = varargin(2): list() of functions or builtin handles or :
    //
    // Called for all generic types: booleans, integers,
    //  decimal or complex numbers, polynomials, texts

    // INITIALIZATIONS
    // ---------------
    lhs = argn(1)
    sizes = size(A)
    L = prod(sizes(3:$))
    AisBool = typeof(A)=="boolean"
    if AisBool
        A = iconvert(A,1) // int8
    end

    // CHECKING PARAMETERS
    // -------------------
    // method: checked in the gateway
    if ~isdef("method","l")
        method = "g"
    elseif method == "r"
        method = 1
    elseif method == "c"
        method = 2
    end
    // sortdir, criteria: checked in each overload, since these ones
    //      can also be called directly by the gateway

    // PREPROCESSING
    // -------------
    if type(method)==1 then
        // We permute dims to make #1 the dim along which to sort
        if method > 1
            d = 1:ndims(A)
            d([1 method]) = [method 1]
            A = permute(A, d)
        end
        sA = size(A)
        A = matrix(A, size(A,1), -1)
    elseif or(method==["lr" "lc"])
        // transform input hypermatrix to a hypermatrix of 3 dimensions
        A = matrix(A, sizes(1), sizes(2), -1)
        // init output variables
        S = zeros(sizes(1), sizes(2), L)
    end

    // PROCESSING
    // ==========
    // ONLY THE SORTED ARRAY IS EXPECTED (gsort is then faster)
    // --------------------------------------------------------
    if(lhs == 1)
        if method == "g"
            S = gsort(A(:), method, varargin(:))

        elseif or(method==["lr" "lc"])
            // perform a 2D sort for each sheet
            for i = 1:L
                S(:,:,i) = gsort(A(:,:,i), method, varargin(:))
            end

        else    // sorting along a dimension
            S = gsort(A, "r", varargin(:))
            S = matrix(S, sA)
            if method > 1
                S = permute(S, d)
            end
        end

    // INDICES ARE ALSO EXPECTED
    // -------------------------
    else
        if method == "g"
            [S, k] = gsort(A(:), method, varargin(:))
            k = matrix(k, size(A))

        elseif or(method==["lr" "lc"])
            sizesInd = sizes
            if method == "lc"
                sizesInd(1) = 1
            elseif method == "lr"
                sizesInd(2) = 1
            end
            k = zeros(sizesInd(1), sizesInd(2), L)

            // perform a 2D sort for each sheet
            for i = 1:L
                [S(:,:,i), k(:,:,i)] = gsort(A(:,:,i), method, varargin(:))
            end
            // return the result with the good dimensions
            k = matrix(k, sizesInd)

        else    // sorting along a dimension
            [S, k] = gsort(A, "r", varargin(:))
            S = matrix(S, sA)
            k = matrix(k, sA)
            if method > 1
                S = permute(S, d)
                k = permute(k, d)
            end

        end
    end
    S = matrix(S, sizes)
    if AisBool then
        S = S==int8(1)
    end
endfunction
