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
// <-- Non-regression test for bug 15692 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15692
//
// <-- Short Description -->
// listvarinfile() crashes when listing a boolean sparse matrix (regression)

x = matrix(1:100, [10, 10]);
b = modulo(x, 5) == 0;
spb = sparse(b);

path = TMPDIR + "/bug_15692.dat";
save(path, "spb");
[names, typs, dims, vols] = listvarinfile(path);
assert_checkequal(names, "spb");
assert_checkequal(typs, type(spb));
assert_checkequal(dims(1), size(spb));
assert_checkequal(vols, 120);
