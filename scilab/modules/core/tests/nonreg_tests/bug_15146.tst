// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 15146 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15146
//
// <-- Short Description -->
// lasterror() crashes Scilab when last error message is empty

execstr("error("""")", "errcatch");
assert_checkequal(lasterror(), "");
