// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyrifht (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [S, k] = %hm_gsort(A, optsort, directionsort)
    rhs = argn(2);
    lhs = argn(1);
    // arguments by default in gsort
    select rhs
    case 1
        optsort = "g";
        directionsort = "d";
    case 2
        // optsort can be: 'r', 'c', 'g', 'lr', 'lc'
        pos_opts = find(optsort == ["r", "c", "g", "lr", "lc"]);
        if pos_opts == [] then
            error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"), "gsort", 2, "r", "c", "g", "lr", "lc"));
        end
        directionsort = "d";
    else
        // optsort can be: 'r', 'c', 'g', 'lr', 'lc'
        pos_opts = find(optsort == ["r", "c", "g", "lr", "lc"]);
        // directionsort can be: 'd' or 'i'
        pos_direction = find(directionsort == ["d", "i"]);
        if pos_opts == [] then
            error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"), "gsort", 2, "r", "c", "g", "lr", "lc"));
        end
        if pos_direction == [] then
            error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), "gsort", 3, "d", "i"));
        end
    end

    if(lhs == 1) // without output indices, gsort is faster.
        if optsort == "g"
            S = gsort(A(:), optsort, directionsort);
            S = matrix(S, size(A));
        else // 'r' 'c' 'lr' 'lc'
            sizes = size(A);

            // transform input hypermatrix to a hypermatrix of 3 dimensions
            l = prod(sizes(3:$));
            mat = matrix(A,sizes(1), sizes(2), l);

            // init output variables
            S = zeros(sizes(1), sizes(2), l);

            // perform a 2D sort for each dimensions > 2
            for i = 1:l
                S(:,:,i) = gsort(mat(:,:,i), optsort, directionsort);
            end

            // return the result with the good dimensions
            S = matrix(S, sizes);
        end
    else
        if optsort == "g"
            [S, k] = gsort(A(:), optsort, directionsort);
            S = matrix(S, size(A));
            k = matrix(k, size(A));
        else // 'r' 'c' 'lr' 'lc'
            sizes = size(A);
            sizesInd = size(A);

            // transform input hypermatrix to a hypermatrix of 3 dimensions
            l = prod(sizes(3:$));
            mat = matrix(A,sizes(1), sizes(2), l);

            // init output variables
            S = zeros(sizes(1), sizes(2), l);
            if optsort == "lc"
                sizesInd(1) = 1;
            elseif optsort == "lr"
                sizesInd(2) = 1;
            end
            k = zeros(sizesInd(1), sizesInd(2), l);

            // perform a 2D sort for each dimensions > 2
            for i = 1:l
                [S(:,:,i), k(:,:,i)] = gsort(mat(:,:,i), optsort, directionsort);
            end

            // return the result with the good dimensions
            S = matrix(S, sizes);
            k = matrix(k, sizesInd);
        end
    end
endfunction
