// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10428 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10428
//
// <-- Short Description -->
//
// Java apps shew a library load error in CLI

assert_checkerror("xcos", msprintf(_("Scilab ''%s'' module disabled in -nogui or -nwni mode.\n"), "xcos"));
assert_checkerror("scinotes", msprintf(_("Scilab ''%s'' module disabled in -nogui or -nwni mode.\n"), "scinotes"));
assert_checkerror("browsehistory", msprintf(_("Scilab ''%s'' module disabled in -nogui or -nwni mode.\n"), "history_browser"));
assert_checkerror("browsevar", msprintf(_("Scilab ''%s'' module disabled in -nogui or -nwni mode.\n"), "ui_data"));
assert_checkerror("filebrowser", msprintf(_("Scilab ''%s'' module disabled in -nogui or -nwni mode.\n"), "ui_data"));
assert_checkerror("editvar(''a'')", msprintf(_("Scilab ''%s'' module disabled in -nogui or -nwni mode.\n"), "ui_data"));
assert_checkerror("driver(''png'')", msprintf(_("Scilab ''%s'' module disabled in -nogui or -nwni mode.\n"), "graphic_export"));
assert_checkerror("plot2d()", msprintf(_("Scilab ''%s'' module disabled in -nogui or -nwni mode.\n"), "graphics"));