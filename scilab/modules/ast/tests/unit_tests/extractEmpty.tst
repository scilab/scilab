// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

a = [];
assert_checkequal(a(5), a);
assert_checkequal(a(:), a);
assert_checkequal(a(1:2), a);
assert_checkequal(a(:,:), a);
assert_checkequal(a(1:2,1:2), a);
assert_checkequal(a(%t), a);
assert_checkequal(a(%f), a);

a = {};
assert_checkequal(a(5), a);
assert_checkequal(a(:), a);
assert_checkequal(a(1:2), a);
assert_checkequal(a(:,:), a);
assert_checkequal(a(1:2,1:2), a);
assert_checkequal(a(%t), a);
assert_checkequal(a(%f), a);

a = struct();
assert_checkequal(a(5), a);
assert_checkequal(a(:), a);
assert_checkequal(a(1:2), a);
assert_checkequal(a(:,:), a);
assert_checkequal(a(1:2,1:2), a);
assert_checkequal(a(%t), a);
assert_checkequal(a(%f), a);

a = ones(5,5);
assert_checkequal(a(), a);

a = cell(5,5);
errmsg = msprintf(gettext("Cell : Cannot extract without arguments.\n"));
assert_checkerror("a{}", errmsg);

a = struct();
assert_checkequal(a(), a);
