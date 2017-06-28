// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for mean()
// =============================================================================
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->

assert_checkequal(mean(0), 0);
assert_checkequal(mean(zeros(3,3)), 0);
assert_checkequal(mean(zeros(3,3,3)), 0);
assert_checkequal(mean(1, 1), 1);
assert_checkequal(mean(1, 2), 1);


// Overloading
m = rand(3,4)*100;
    // for a built-in type
function s = %i_mean(ob, varargin)
    s = mean(double(ob), varargin(:));
endfunction
ob = uint8(m);
r = fix(m);
assert_checkequal(mean(ob),mean(r));
assert_checkequal(mean(ob, 1), mean(r, 1));
assert_checkequal(mean(ob, 2), mean(r, 2));
    // for a typed list
function s = %mytype_size(ob, varargin)
    s = size(ob.mat, varargin(:));
endfunction
function s = %mytype_mean(ob, varargin)
    s = mean(ob.mat, varargin(:));
endfunction
ob = mlist(["mytype","mat","title"], m, "A test object");
assert_checkequal(mean(ob),mean(m));
assert_checkequal(mean(ob, 1), mean(m, 1));
assert_checkequal(mean(ob, 2), mean(m, 2));


