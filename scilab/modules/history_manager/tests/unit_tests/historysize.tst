//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

savehistory(TMPDIR + "/historysize_backup"); // backup

resethistory();
assert_checkequal(historysize(), 0);

fmterr = msprintf(gettext("%s: Wrong value for input argument #%d: ""%s"" expected.\n"),"historysize", 1, "max");
assert_checkerror ("historysize(""toto"")", fmterr , 999 );

assert_checkequal(historysize(100), 100);
assert_checkequal(historysize("max"), 100);

fmterr = msprintf(gettext("%s: Wrong value for input argument #%d.\n"),"historysize", 1);
assert_checkerror ("historysize(-1)", fmterr , 999 );

strs = string(1:1000);
resethistory();
addhistory(strs);
savehistory(TMPDIR + "/historysize_tmp");

resethistory();
warning("off");
loadhistory(TMPDIR + "/historysize_tmp");
assert_checkequal(historysize(), 100 + 1);

resethistory();
loadhistory(TMPDIR + "/historysize_backup"); // restore
