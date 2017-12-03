// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15302 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15302
//
// <-- Short Description -->
// Wrong load() calling sequence can crash Scilab!

a = 1;
save("TMPDIR/file.sod", "a");
msg = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "load", 2);
assert_checkerror("load(""TMPDIR/file.sod"", a);", msg);
