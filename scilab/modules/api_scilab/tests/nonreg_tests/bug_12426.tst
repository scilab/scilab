// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 12426 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12426
//
// <-- Short Description -->
// By using addErrorMessage, random string error can be displayed.

ilib_verbose(0);
dir = get_absolute_file_path("bug_12426.tst");
exec(dir + "/build_bug_12426_gateway.sce", "errcatch", -1);
exec(TMPDIR + "/bug_12426_gateway/loader.sce", "errcatch", -1);

n = int32(1);
disp("add 1 error message");
addErrorMessage(n);

n = int32(4);
disp("add 4 error messages");
addErrorMessage(n);

n = int32(5);
disp("add 5 error messages");
addErrorMessage(n);

n = int32(6);
disp("add 6 error messages");
addErrorMessage(n);
