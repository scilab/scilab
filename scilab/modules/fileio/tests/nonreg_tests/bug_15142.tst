// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Nimish Kapoor
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15142 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=15142
//
// <-- Short Description -->
// mopen(): wrong err value

[fd, err] = mopen('fake-file.txt', 'r')
assert_checkequal(err,-2);