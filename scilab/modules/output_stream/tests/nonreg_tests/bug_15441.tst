// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15441 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15441
//
// <-- Short Description -->
// mprintf("%%") crashes scilab

// Test done with msprintf: msprintf and mprintf gateways both call the fautly scilab_sprintf, where the bug is fixed
assert_checkequal(msprintf("%%"),"%")
assert_checkequal(msprintf("x=%%pi"),"x=%pi")

test_file = TMPDIR+'/bug_15441.txt';
fd = mopen(test_file,'a');
mfprintf(fd, "%%");
mfprintf(fd, "x=%%pi");
mclose(fd);
assert_checkequal(mgetl(test_file),"%x=%pi")
