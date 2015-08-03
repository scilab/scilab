// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->

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
