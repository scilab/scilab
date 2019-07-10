// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16135 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16135
//
// <-- Short Description -->
// base2dec does not detect invalid numbers

message = msprintf(_("%s: Wrong value for input argument #%d: Valid base %d representations expected.\n"), "base2dec", 1, 2);
assert_checkerror("base2dec(""3"",2)", message)