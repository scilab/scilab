// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2014, 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = %s_k_s(a, b)
    // The b block is replicated according to the a's size and values as weights

    // Code used instead of %s_k_hm(), %hm_k_s() and %hm_k_hm()

    // Computes the size of the result
    sa = size(a); na = length(sa);
    sb = size(b); nb = length(sb);
    m = max(na, nb);
    if na < m
        sa = [sa ones(1,m-na)]
    else
        sb = [sb ones(1,m-nb)]
    end
    sc = sa .* sb;

    // Computes the matrice of indices shifts within each block, in the result:
    // We use the first block
    ijk = ind2sub(size(b), 1:size(b,"*"))
    if na > nb then
        ijk = [ijk ones(size(b,"*"), na-nb)]
    end
    shifts = sub2ind(sc, ijk) - 1;

    // Computes the index of the first element of each block, for all blocks in
    //  the result:
    ijk = ind2sub(size(a), 1:size(a,"*")) // indices in a
    ijk = ijk - 1
    for u = 1:size(ijk, 2)
        ijk(:,u) = ijk(:,u) * sb(u)
    end
    if na < nb then
        ijk = [ijk zeros(size(a,"*"), nb-na)]
    end
    first = sub2ind(sc, ijk + 1)

    // Computes new indices
    newI = shifts(:) * ones(1, size(a,"*"))     // Replicates shifts
    newI = newI(:) + ..
          (ones(size(b,"*"),1) * first(:)')(:) // Replicates base indices (1st elements)
    clear shifts first ijk

    // Replicates and weights data
    r = b(:) * matrix(a, 1, -1);

    // Reallocates elements
    r(newI) = r(:)
    clear newI

    // Reshape the result
    r = matrix(r, sc);
endfunction
