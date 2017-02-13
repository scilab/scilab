// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// test xset and xget

// fpf
out = xget("fpf");
assert_checkequal(out, "");
xset("fpf", "%s.%s");
out = xget("fpf");
assert_checkequal(out, "%s.%s");

// XSET
ierr = execstr("xset(""fpf"");","errcatch");
assert_checkequal(ierr, 77);
