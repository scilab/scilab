// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises -
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
//<-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14177 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14177
//
// <-- Short Description -->
// Using Java interface functions crashes Scilab in NWNI mode
//

ierr = execstr("jcompile()", "errcatch");
assert_checkequal(ierr, 999);
