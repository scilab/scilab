// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI Group - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 15464 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15464
//
// <-- Short Description -->
// Xcos C-blocks could not link dlls on Windows

copyfile(SCI+"/modules/scicos_blocks/tests/nonreg_tests/bug_15464/foo.h", TMPDIR);
copyfile(SCI+"/modules/scicos_blocks/tests/nonreg_tests/bug_15464/foo.c", TMPDIR);

// Create TMPDIR/foo.dll & TMPDIR/foo.lib
oldPath = pwd();
cd(TMPDIR);
ilib_for_link("foo", "foo.c", [], "c");
cd(oldPath);

xcos(SCI+"/modules/scicos_blocks/tests/nonreg_tests/bug_15464/call_lib.zcos");

// Edit the CBLOCK4 to include TMPDIR/foo.h in the C code and TMPDIR/foo in External libraries
// Check that compilation + link passes and simulation runs to show the result: 4
