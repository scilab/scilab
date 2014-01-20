// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7244 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7244
//
// <-- Short Description -->
// Extraction from a struct array with a boolean vector had a strange behavior.

foo(1).bar = 1;
foo(2).bar = 2;
foo(3).bar = 3;        // 3x1 struct array with field: bar.
foo = foo([%T %F %T]); // 2x1 struct array with field: bar.
assert_checkequal(type(foo), 17);

foo(3).bar = 2;
assert_checkequal(foo.bar(1), 1);
assert_checkequal(foo.bar(2), 3);
assert_checkequal(foo.bar(3), 2);
