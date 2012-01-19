// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK ERROR OUTPUT -->
// <-- ENGLISH IMPOSED -->

// Test the Xcos toolbox skeleton in SCI/contrib/
//  - Building/Compilation
//  - Loading
//  - Tests

// Xcos external module skeleton
status = copyfile(SCI + "/contrib/xcos_toolbox_skeleton/", TMPDIR + "/xcos_toolbox_skeleton/");
assert_checkequal(status, 1);
exec("TMPDIR/xcos_toolbox_skeleton/builder.sce");
exec("TMPDIR/xcos_toolbox_skeleton/loader.sce");
test_run("TMPDIR/xcos_toolbox_skeleton/", [], "short_summary");