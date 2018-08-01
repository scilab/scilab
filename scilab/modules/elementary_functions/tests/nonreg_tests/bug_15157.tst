// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug #15157 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15157
//
// <-- Short Description -->
// min/max on tlist or mlist did not call good overload.

function m = %test_max(varargin)
    x = varargin(1);
    m = max(x.values);
endfunction

function m = %test_min(varargin)
    x = varargin(1);
    m = min(x.values);
endfunction

x = mlist(["test", "values"], 1:10);

assert_checkequal(min(x), 1);
assert_checkequal(max(x), 10);

