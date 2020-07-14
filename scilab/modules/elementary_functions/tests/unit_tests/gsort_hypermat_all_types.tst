// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) ????-2008 - ENPC
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2018 - 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//================ Sorting hypermatrices (all data types) ======================

A = rand(6, 4, 2, 8);
for o = ["d" "i"]
    // 'g'
    [hs hind]   = gsort(A, 'g', o);
    [s ind]     = gsort(A(:), 'g', o);

    assert_checkequal(hs(:), s);
    assert_checkequal(hind(:), ind);

    hs = gsort(A, 'g', o);
    assert_checkequal(hs(:), s);

    // 'c' 'r' 'lc' 'lr'
    [hsc hindc]   = gsort(A, "c", o);
    [hsr hindr]   = gsort(A, "r", o);
    [hslc hindlc] = gsort(A, "lc", o);
    [hslr hindlr] = gsort(A, "lr", o);

    a       = matrix(A, 6, 4, 16); // 2 * 8 = 16
    z = zeros(a);
    [sc, indc, sr, indr, slc, slr] = (z, z, z, z, z, z);
    indlc   = zeros(1, 4, 16);
    indlr   = zeros(6, 1, 16);

    for i = 1:16
        [sc(:,:,i) indc(:,:,i)]   = gsort(a(:,:,i),"c", o);
        [sr(:,:,i) indr(:,:,i)]   = gsort(a(:,:,i),"r", o);
        [slc(:,:,i) indlc(:,:,i)] = gsort(a(:,:,i),"lc", o);
        [slr(:,:,i) indlr(:,:,i)] = gsort(a(:,:,i),"lr", o);
    end

    sc      = matrix(sc, 6, 4, 2, 8);
    indc    = matrix(indc, 6, 4, 2, 8);
    sr      = matrix(sr, 6, 4, 2, 8);
    indr    = matrix(indr, 6, 4, 2, 8);
    slc     = matrix(slc, 6, 4, 2, 8);
    indlc   = matrix(indlc, 1, 4, 2, 8);
    slr     = matrix(slr, 6, 4, 2, 8);
    indlr   = matrix(indlr, 6, 1, 2, 8);

    assert_checkequal(hsc, sc);
    assert_checkequal(hindc, indc);
    assert_checkequal(hsr, sr);
    assert_checkequal(hindr, indr);
    assert_checkequal(hslc, slc);
    assert_checkequal(hindlc, indlc);
    assert_checkequal(hslr, slr);
    assert_checkequal(hindlr, indlr);

    // whitout indices
    hsc  = gsort(A, "c", o);
    hsr  = gsort(A, "r", o);
    hslc = gsort(A, "lc", o);
    hslr = gsort(A, "lr", o);

    assert_checkequal(hsc, sc);
    assert_checkequal(hsr, sr);
    assert_checkequal(hslc, slc);
    assert_checkequal(hslr, slr);
end

// Sorting along a dim > 2
// -----------------------
H = grand(2,4,3,3,"uin",-9,9);
function r = toBool(m), r = m < 0; endfunction
for fun = list(toBool, int8, uint8, int16, uint32, int64, string)
    h = fun(H);
    for o = ["i" "d"]
        // along dim = 1 (checking)
        [sh, k] = gsort(h, 1, o);
        for i2 = 1:size(h,2)
            for j2 = 1:size(h,3)
                for k2 = 1:size(h,4)
                    [s, ks] = gsort(h(:,i2,j2,k2), "g", o);
                    assert_checkequal(s, sh(:,i2,j2,k2));
                    assert_checkequal(ks, k(:,i2,j2,k2));
                end
            end
        end
        // along dim = 3
        [sh, k] = gsort(h, 3, o);
        for i2 = 1:size(h,1)
            for j2 = 1:size(h,2)
                for k2 = 1:size(h,4)
                    [s, ks] = gsort(h(i2,j2,:,k2), "g", o);
                    assert_checkequal(s, sh(i2,j2,:,k2));
                    assert_checkequal(ks, k(i2,j2,:,k2));
                end
            end
        end
        // along dim = 4
        [sh, k] = gsort(h, 4, o);
        for i2 = 1:size(h,1)
            for j2 = 1:size(h,2)
                for k2 = 1:size(h,3)
                    [s, ks] = gsort(h(i2,j2,k2,:), "g", o);
                    assert_checkequal(s, sh(i2,j2,k2,:));
                    assert_checkequal(ks, k(i2,j2,k2,:));
                end
            end
        end
    end
end
