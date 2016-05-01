// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

l = list(1, "string", %s);
setfield(2, "newString", l);
assert_checkequal(l, list(1, "newString", %s));
setfield(0, "front", l);
assert_checkequal(l, list("front", 1, "newString", %s));

// b must not be modified
a = list(2);
b = a;
setfield(1, 15, a);
assert_checkequal(a, list(15));
assert_checkequal(b, list(2));
