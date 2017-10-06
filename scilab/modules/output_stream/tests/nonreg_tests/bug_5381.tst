// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5381 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/5381
//
// <-- Short Description -->
// With UTF-8 inputs, msprintf(), mprintf() and fprintf() output were shortened
//

assert_checkequal(msprintf("% 6s", "é"), "     é");
assert_checkequal(msprintf("% 6s", "éé"), "    éé");

mprintf("% 6s", "é");
mprintf("% 6s", "éé");

fd = mopen(TMPDIR+'/bug_5381.txt','wt');
mfprintf(fd, "% 6s\n", "é")
mfprintf(fd, "% 6s\n", "éé")
mclose(fd);
t = mgetl(TMPDIR+'/bug_5381.txt');
assert_checkequal(t, ["     é";"    éé"]);
