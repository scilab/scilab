// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI-Group - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15692 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15692
//
// <-- Short Description -->
// listvarinfile() crashes when listing a boolean sparse matrix (regression)

spb = sprand(4,10,0.1) < 0.5;
spbc = spb;
path = TMPDIR + "/test.dat";
save(path, "spb");
listvarinfile(path);
assert_checkequal(spb, spbc);