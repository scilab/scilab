// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - ESI-Group - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15693 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15693
//
// <-- Short Description -->
// listvarinfile() crashes when listing a boolean sparse matrix (regression)

p = (1-%z)^[1 2];
path = TMPDIR + "/bug_15693.dat";
save(path, "p")
[names, typs, dims, vols] = listvarinfile(path);
assert_checkequal(names, "p");
assert_checkequal(typs, type(p));
assert_checkequal(dims(1), size(p));
assert_checkequal(vols, 40);
