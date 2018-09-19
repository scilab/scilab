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
// <-- Non-regression test for bug 15502 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15502
//
// <-- Short Description -->
// listvarinfile() misses listing arrays of struct, and then locks the file.

s(2,3).r = %e;
path = TMPDIR+"/bug_15502.sod";
save(path, "s");
[names, typs, dims, vols] = listvarinfile(path);
assert_checkequal(names, "s");
assert_checkequal(typs, type(s));
assert_checkequal(dims(1), size(s));
assert_checkequal(vols, 8);