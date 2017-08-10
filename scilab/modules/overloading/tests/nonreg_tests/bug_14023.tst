// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 14023 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14023
//
// <-- CLI SHELL MODE -->
//
// <-- Short Description -->
// Concatenate cells is not possible

// Rows
ref = makecell([1,3], "aaa", "bbb", "ccc");
assert_checkequal([cellstr("aaa"), cellstr("bbb"), cellstr("ccc")], ref);

// Columns
ref = makecell([3,1], "aaa", "bbb", "ccc");
assert_checkequal([cellstr("aaa"); cellstr("bbb"); cellstr("ccc")], ref);

// Transpose
c = makecell([3,3], "a", "b", "c", "d", "e", "f", "g", "h", "i");
ref = makecell([3,3], "a", "d", "g", "b", "e", "h", "c", "f", "i");
assert_checkequal(c', ref);
