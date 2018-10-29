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
    // method can be: 'g', 'r', 'c', 'lr', 'lc'
    // sortdir = varargin(1): "i", "d", or vector of "i" and "d"
    // criteria = varargin(2): list() of functions or builtin handles or :
    //
    // Called for all generic types: booleans, integers,
    //  decimal or complex numbers, polynomials, texts

    // INITIALIZATIONS
    lhs = argn(1)
    sizes = size(A)
    L = prod(sizes(3:$))
    AisBool = typeof(A)=="boolean"
    if AisBool
        A = iconvert(A,1) // int8
    end

    // CHECKING PARAMETERS
    // method: checked in the gateway
    if ~isdef("method","l")
        method = "g"
    end
    // sortdir, criteria: checked in each overload, since these ones
    //      can also be called directly by the gateway

    // ONLY THE SORTED ARRAY IS EXPECTED (gsort is then faster)
    if(lhs == 1)
        if method == "g"
            S = gsort(A(:), method, varargin(:))
            S = matrix(S, size(A))
        else // 'r' 'c' 'lr' 'lc'
            // transform input hypermatrix to a hypermatrix of 3 dimensions
            mat = matrix(A,sizes(1), sizes(2), -1)

            // init output variables
            S = zeros(sizes(1), sizes(2), L)

            // perform a 2D sort for each sheet
            for i = 1:L
                S(:,:,i) = gsort(mat(:,:,i), method, varargin(:))
            end
        end

    // INDICES ARE ALSO EXPECTED
    else
        if method == "g"
            [S, k] = gsort(A(:), method, varargin(:))
            S = matrix(S, size(A))
            k = matrix(k, size(A))
        else // 'r' 'c' 'lr' 'lc'
            sizesInd = size(A)

            // transform input hypermatrix to a hypermatrix of 3 dimensions
            mat = matrix(A,sizes(1), sizes(2), -1)

            // init output variables
            S = zeros(sizes(1), sizes(2), L)
            if method == "lc"
                sizesInd(1) = 1
            elseif method == "lr"
                sizesInd(2) = 1
            end
            k = zeros(sizesInd(1), sizesInd(2), L)

            // perform a 2D sort for each sheet
            for i = 1:L
                [S(:,:,i), k(:,:,i)] = gsort(mat(:,:,i), method, varargin(:))
            end

            // return the result with the good dimensions
            k = matrix(k, sizesInd)
        end
    end
    S = matrix(S, sizes)
    if AisBool then
        S = S==int8(1)
    end
endfunction
