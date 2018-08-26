// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15728 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15728
//
// <-- Short Description -->
// evstr("k") evstr("tmp") evstr("comm") evstr("lhs") evstr("rhs") ...
// were wrong (overwritten by internal values).

k = rand(1,3);
tmp = k; lhs = k; rhs = k; comm = k; vals = k; vars = k;
for s = ["k" "tmp" "lhs" "rhs" "comm" "vals" "vars"]
    assert_checkequal(evstr(s), k);
end
assert_checkequal(evstr("k;tmp;lhs;rhs;comm;vals;vars"), ones(7,1)*k);
