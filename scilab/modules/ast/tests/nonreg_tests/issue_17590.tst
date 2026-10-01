// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for issue 17590 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Gitlab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17590
//
// <-- Short Description -->
// arguments: vectors are not transposed for size specs with a colon, e.g. (:,1) and (1,:)

function sz = a1(x)
    arguments
        x (:,1) double
    end
    sz = size(x)
endfunction

function sz = a2(x)
    arguments
        x (1,:) double
    end
    sz = size(x)
endfunction

assert_checkequal(a1([1 2 3]), [3 1]);
assert_checkequal(a2([1;2;3]), [1 3]);
