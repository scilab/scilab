// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH ATOMS -->
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10933 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10933
//
// <-- Short Description -->
// atomsGetConfig did not check value of input argument

msgerr = msprintf(gettext("%s: Wrong value for input argument #%d.\n"),"atomsGetConfig", 1);
assert_checkerror("atomsGetConfig(''blabla'')", msgerr);

atomsGetConfig("useProxy");
atomsGetConfig("proxyHost");
atomsGetConfig("proxyPort");
atomsGetConfig("proxyUser");
atomsGetConfig("proxyPassword");
atomsGetConfig("offline");
atomsGetConfig("autoload");
atomsGetConfig("autoloadAddAfterInstall");
atomsGetConfig("verbose");
atomsGetConfig("downloadTool");
atomsGetConfig("downloadTimeout");