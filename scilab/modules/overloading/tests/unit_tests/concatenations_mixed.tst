// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018-2020 - Samuel GOUGEON - Le Mans Université
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit tests for mixed types concatenations
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// -----------------------------
// Booleans and encoded integers
// -----------------------------
for i = [1 2 4 8 11 12 14 18]  // loop on inttype codes
    r = [iconvert(-3,i) , %t];
    r2 = [%t , iconvert(-3,i)];
    c2 = [%t ; iconvert(-3,i)];
    c = [iconvert(-3,i) ; %t];
    assert_checkequal(inttype(r), i);
    assert_checkequal(inttype(r2), i);
    assert_checkequal(inttype(c), i);
    assert_checkequal(inttype(c2), i);
    assert_checkequal(size(r) , [1 2]);
    assert_checkequal(size(r2), [1 2]);
    assert_checkequal(size(c) , [2 1]);
    assert_checkequal(size(c2), [2 1]);
end

// ----------------------------
// Doubles and encoded integers
// ----------------------------
D = list(2, [3 7], [3 ; 7], [1 2 3 ; 4 5 6], cat(3,[1 2 3;4 5 6],[8 4 9;0 3 1]));
for it = [1 2 4 8 11 12 14 18]  // Loop on integer types
    for d = D
        assert_checkequal([d, iconvert(d,it)], [d d]);
        assert_checkequal([iconvert(d,it), d], [d d]);
        assert_checkequal([d ; iconvert(d,it)], [d ; d]);
        assert_checkequal([iconvert(d,it) ; d], [d ; d]);
    end
end

// -------------------------------------------------
// A boolean and a double, at least one being sparse
// -------------------------------------------------
s = [2 -1 3 ; 4 0 2];
sp = sparse(s);
b = [%T %F %F ; %F %T %T];
spb = sparse(b);
objects = list(s, sp, b, spb);
for a = objects
    for b = objects
        if (or(type(a)==[1 5]) & or(type(b)==[1 5])) | ..
            (or(type(a)==[4 6]) & or(type(b)==[4 6])) | ..
            (~issparse(a) & ~issparse(b))
            continue
        end
        c = [a b];
        if or(type(a)==[1 5]) | or(type(b)==[1 5])
            assert_checkequal(type(c), 5);
        else
            assert_checkequal(type(c), 6);
        end
        assert_checkequal(size(c), [2 6]);

        c = [a ; b];
        if or(type(a)==[1 5]) | or(type(b)==[1 5])
            assert_checkequal(type(c), 5);
        else
            assert_checkequal(type(c), 6);
        end
        assert_checkequal(size(c), [4 3]);
    end
end
