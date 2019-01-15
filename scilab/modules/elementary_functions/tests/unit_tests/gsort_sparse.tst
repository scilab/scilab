// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) ????-2008 - ENPC
// Copyright (C) 2008 - DIGITEO - Allan CORNET
// Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//================================ sparse vectors ==============================

sp = sparse([1,2,4,5,3,10]);
ref = sparse([10,5,4,3,2,1]);
A = gsort(sp);
assert_checkequal(ref, A);

sp = sparse([1,2;4,5;3,10]);
assert_checkfalse(execstr("A = gsort(sp)", "errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: sparse vectors expected.\n"), "gsort", 1);
assert_checkerror("A = gsort(sp)", refMsg);

//=============== testing gsort with Sparse (macro: %sp_gsort) =================

// with real sparses :
b = [5 1 3 2 4;6 1 2 4 1]; b = b(:);
B = sparse(b);
for smode = ["g" "r" "c" "lr" "lc"]
    for sdir = ["d" "i"]
        [s, k] = gsort(b, smode, sdir);
        [s1, k1] = gsort(B, smode, sdir);
        assert_checkequal(full(s1),s);
        assert_checkequal(k1,k);
        [s, k] = gsort(b', smode, sdir);
        [s1, k1] = gsort(B', smode, sdir);
        assert_checkequal(full(s1),s);
        assert_checkequal(k1,k);
    end
end

// with complex sparses :
A = [1 -%i; %i 0]; A = A(:);
A1 = sparse(A);
B = [1 1+%i 4; -2*%i 3 3-%i]; B = B(:);
B1 = sparse(B);
C = [-%i 3*%i; 4 9; -2*%i 7]; C = C(:);
C1 = sparse(C);
for smode = ["g" "r" "c"]
    for sdir = ["d" "i"]
        c = gsort(A, smode, sdir);
        d = full(gsort(A1, smode, sdir));
        assert_checkequal(c,d);
        c = gsort(B, smode, sdir);
        d = full(gsort(B1, smode, sdir));
        assert_checkequal(c,d);
        [s, k] = gsort(C, smode, sdir);
        [s1, k1] = gsort(C1, smode, sdir);
        assert_checkequal(full(s1),s);
        assert_checkequal(k1,k);
    end
end
