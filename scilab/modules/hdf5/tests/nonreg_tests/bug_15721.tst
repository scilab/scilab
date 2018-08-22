// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15721 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15721
//
// <-- Short Description -->
// An error while save(File, "-append", ..) deleted the existing file.

a = 1;
iL = 1:$;  // can't be saved => will yield an error
File = TMPDIR + filesep() + "bug_15721.sod";
save(File, "a");
try
    save(File, "-append", "iL");
end
assert_checktrue(isfile(File));
assert_checkequal(listvarinfile(File), "a");
mdelete(File);

// ----
try
    save(File, "-append", "iL");
end
assert_checkfalse(isfile(File));

// ----
try
    save(File, "-append", "a", "iL");
end
assert_checkfalse(isfile(File));
