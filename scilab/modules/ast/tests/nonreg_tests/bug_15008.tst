// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 15008 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15008
//
// <-- Short Description -->
// scilab crash in using operator AND (&, &&) or OR (| ||) with a string.

b_and_c = msprintf(_("Undefined operation for the given operands.\n") + "%s", msprintf(_("check or define function %s for overloading.\n"), "%b_h_c"));
c_and_b = msprintf(_("Undefined operation for the given operands.\n") + "%s", msprintf(_("check or define function %s for overloading.\n"), "%c_h_b"));
b_or_c  = msprintf(_("Undefined operation for the given operands.\n") + "%s", msprintf(_("check or define function %s for overloading.\n"), "%b_g_c"));
c_or_b  = msprintf(_("Undefined operation for the given operands.\n") + "%s", msprintf(_("check or define function %s for overloading.\n"), "%c_g_b"));

assert_checkerror("if %t & ""0"" then, end;", b_and_c);
assert_checkerror("if %t && ""0"" then, end;", b_and_c);
assert_checkerror("if ""0"" & %t then, end;", c_and_b);
assert_checkerror("if ""0"" && %t then, end;", c_and_b);

assert_checkerror("if %f | ""0"" then, end;", b_or_c);
assert_checkerror("if %f || ""0"" then, end;", b_or_c);
assert_checkerror("if ""0"" | %t then, end;", c_or_b);
assert_checkerror("if ""0"" || %t then, end;", c_or_b);
