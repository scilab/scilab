// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a = list("cos", [1.1 2]);
b = var2vec(a);
c = vec2var(b);
assert_checkequal(type(c), 15);
assert_checkequal(c(1),    "cos");


// Error checks
b(9) = 1000; // Not a Scilab type
refMsg = msprintf(_("%s: Wrong value for element #%d of input argument #%d: Unknown type.\n"), "vec2var", 9, 1);
assert_checkerror("vec2var(b)", refMsg);

b(3) = 1000; // Not a Scilab type
refMsg = msprintf(_("%s: Wrong value for element #%d of input argument #%d: Unknown type.\n"), "vec2var", 3, 1);
assert_checkerror("vec2var(b)", refMsg);

b(1) = 1000; // Not a Scilab type either
refMsg = msprintf(_("%s: Wrong value for element #%d of input argument #%d: Unknown type.\n"), "vec2var", 1, 1);
assert_checkerror("vec2var(b)", refMsg);

a = var2vec(1);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), "vec2var", 1, 6, 1);
assert_checkerror("vec2var(a(1:$-1))", refMsg);

b = var2vec(list(1));
refMsg = msprintf(_("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), "vec2var", 1, 8, 1);
assert_checkerror("vec2var(b(1:$-1))", refMsg);

c = var2vec(list([1 1 1; 1 2 3]));
refMsg = msprintf(_("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), "vec2var", 1, 13, 1);
assert_checkerror("vec2var(c(1:$-1))", refMsg);

c = var2vec(list(list()));
refMsg = msprintf(_("%s: Wrong size for input argument #%d: At least %dx%d expected.\n"), "vec2var", 1, 4, 1);
assert_checkerror("vec2var(c(1:$-1))", refMsg);

d = [10 0 0 0]';
refMsg = msprintf(_("%s: Wrong value for element #%d of input argument #%d: String matrix cannot be empty.\n"), "vec2var", 2, 1);
assert_checkerror("vec2var(d)", refMsg);

e = [8 1 0 0 0]';
refMsg = msprintf(_("%s: Wrong value for element #%d of input argument #%d: Integer matrix cannot be empty.\n"), "vec2var", 3, 1);
assert_checkerror("vec2var(e)", refMsg);
