// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x_out, ka_out, kb_out] = intersect(a_in, b_in, orient)
    // returns the vector of common values of two vectors

    [lhs, rhs] = argn();
    x_out = []
    ka_out = []
    kb_out = []

    if rhs < 2 then
        msg = gettext("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, "intersect", 2, 3))
    end

    if a_in == [] | b_in == [] then
        return
    end
    aIsComplex = type(a_in)==1 && ~isreal(a_in);
    bIsComplex = type(b_in)==1 && ~isreal(b_in);

    // Without orientation
    // -------------------
    if ~isdef("orient","l") then
        if lhs == 1
            a = unique(matrix(a_in, 1, -1));
            b = unique(matrix(b_in, 1, -1));
            if aIsComplex | bIsComplex
                x = gsort([a, b], "g", ["i" "i"], list(abs, atan));
            else
                x = gsort([a, b], "g", "i");
            end
            keq = find( x(2:$) == x(1:$-1) ); // find consecutive equal values index
            if keq <> [] then
                x_out = x(keq); //the intersection values in increasing order
            end
            return
        end
        // lhs > 1
        // .......
        [a, ka] = unique(matrix(a_in, 1, -1));
        [b, kb] = unique(matrix(b_in, 1, -1));
        kab = [ka, -kb];
        // find duplicated values in [a_in,b_in],
        // sort the array
        if aIsComplex | bIsComplex then
            [x, ksort] = gsort([a, b], "g", ["i" "i"], list(abs, atan));
        else
            [x, ksort] = gsort([a, b], "g", "i");
        end
        kab = kab(ksort); // apply [a_in,b_in] sorting permutation to kab
        keq = find( x(2:$) == x(1:$-1) ); // find consecutive equal values index

        if keq <> [] then
            x_out = x(keq); // the intersection values in increasing order
            // each duplicated value appear twice  and only twice and in
            // consecutive positions keq(i) and keq(i)+1 in the sorted array x
            kab = kab([keq keq+1])

            // the positive values correspond to a_in index while the negative to b_in index.
            ka_out = kab(kab > 0) // select index of intersection elements in a_in
            // insure that a_in(ka_out)==x_out and b_in(kb_out)==x_out.
            // I was'nt able to find a simple way.
            if aIsComplex
                [s, k] = gsort(a_in(ka_out), "g", ["i" "i"], list(abs, atan));
            else
                [s, k] = gsort(a_in(ka_out), "g", "i");
            end
            ka_out = ka_out(k)

            if lhs > 2
                kb_out = -kab(kab < 0); //select index of intersection elements in b_in
                if bIsComplex
                    [s, k] = gsort(b_in(kb_out), "g", ["i" "i"], list(abs, atan));
                else
                    [s, k] = gsort(b_in(kb_out), "g", "i");
                end
                kb_out = kb_out(k);
            end
        end  // keq <> []
        return
    end  // rhs < 3

    // WITH an ORIENTATION
    // -------------------
    if ndims(a_in) > 2 | ndims(b_in) > 2 then
        msg = gettext("%s: Argument #%d: Orientation not supported for hypermatrix.\n")
        error(msprintf(msg, "intersect", 3))
    end
    columnwise = orient==2 | orient=="c"
    if columnwise then
        a_in = a_in.'
        b_in = b_in.'
    end
    if and(orient <> [1 2]) & and(orient <> ["r" "c"]) then
        msg = gettext("%s: Argument #%d: Must be in the set {%s}.\n")
        error(msprintf(msg, "intersect", 3, "1,''r'',2,''c''"))
    end
    // row-wise processing
    // -------------------
    if lhs == 1 then
        a = unique(a_in, "r");
        b = unique(b_in, "r");
        if aIsComplex | bIsComplex
            x = gsort([a ; b], "lr", ["i" "i"], list(abs, atan))
        else
            x = gsort([a ; b], "lr", "i")
        end
        keq = find(and(x(2:$,:) == x(1:$-1,:),"c")) // find index of consecutive equal values

        if keq == [] then
            return
        else
            x_out = x(keq,:); //the intersection values in increasing order
        end

    else
        // ka requested : lhs > 1
        // ............
        [a, ka] = unique(a_in, "r");
        [b, kb] = unique(b_in, "r");
        kab = [ka; -kb];
        if aIsComplex | bIsComplex
            [x,ksort] = gsort([a; b], "lr", ["i" "i"], list(abs, atan));
        else
            [x,ksort] = gsort([a; b], "lr", "i");
        end

        kab = kab(ksort);//apply [a_in,b_in] sorting permutation to kab
        keq = find(and(x(2:$,:) == x(1:$-1,:),"c")) // find index of consecutive equal values

        if keq == [] then
            return
        else
            x_out = x(keq,:); //the intersection values in increasing order

            kab = kab([keq keq+1]);

            //the positive values correspond to a_in index while the negative to b_in index.
            ka_out = kab(kab>0); //select index of intersection elements in a_in
            //insure that a_in(ka_out,:)==x_out and b_in(kb_out,:)==x_out.
            //I was'nt able to find a simple way.
            if aIsComplex
                [s, k] = gsort(a_in(ka_out,:), "lr", ["i" "i"], list(abs, atan));
            else
                [s, k] = gsort(a_in(ka_out,:), "lr", "i");
            end
            ka_out = matrix(ka_out(k), 1, -1)

            if lhs > 2 then
                kb_out = -kab(kab<0); //select index of intersection elements in b_in
                if bIsComplex
                    [s, k] = gsort(b_in(kb_out,:), "lr", ["i" "i"], list(abs, atan));
                else
                    [s, k] = gsort(b_in(kb_out,:), "lr", "i");
                end
                kb_out = matrix(kb_out(k),1,-1)
            end
        end
    end

    // Columnwise post-processing
    // ..........................
    if columnwise then
        x_out = x_out.'
    end
endfunction
