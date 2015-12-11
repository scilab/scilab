// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Samuel GOUGEON
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [i_bin, counts, outside] = %c_dsearch(T, bins, discrete)

    // CHECKING PARAMETERS
    // -------------------
    bins2 = unique(bins)
    if size(bins2,"*")~=size(bins,"*")
        msg = _("%c: input argument #%d has duplicates\n")
        error(msprintf(msg, "dsearch", 2))
    end

    // DISCRETE CASE
    // -------------
    if argn(2)>2 & discrete=="d" then
        [nb, i_bin] = members(T, bins)
        outside = sum(bool2s(nb==0))
        counts = zeros(bins)
        inds = [ 0 ; gsort(i_bin(:),"g","i") ; size(bins,"*")+1]
        tmp = find(inds(2:$)~=inds(1:$-1))+1
        counts(inds(tmp(1:$-1))) = tmp(2:$)-tmp(1:$-1)
        counts($) = sum(T==bins($))

        // SORTED CASE
        // -----------
    else
        if or(bins2~=bins)
            msg = _("%c: input argument #%d must be sorted in lexicographic order\n")
            error(msprintf(msg, "dsearch", 2))
        end
        T0 = T
        [T, k] = gsort(T0,"g","i")

        // index of the first T(i) >= bins(1)
        i0 = min(find(strcmp(T(:), bins(1))>=0))
        if i0==[] then
            i0 = 1
        end
        // Initializations
        i_bin = zeros(T)
        counts = zeros(bins(2:$))

        // Loop over the bins
        nT = size(T,"*")
        for i = 1:size(bins,"*")-1
            j = find(strcmp(T(i0:$), bins(i+1))<=0)
            if j==[] then
                counts(i) = 0
            else
                counts(i) = max(j)
                i_bin(i0+j-1) = i
                i0 = i0 + max(j)
            end
            if i0>nT then
                break
            end
        end

        // Taking into account the original T order
        i_bin = i_bin(:)
        i_bin(k(:)) = i_bin
        i_bin = matrix(i_bin, size(k))
        //
        outside = sum(bool2s(i_bin==0))
    end
endfunction
