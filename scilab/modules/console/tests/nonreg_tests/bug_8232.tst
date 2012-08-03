// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 8232 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8232
//
// <-- Short Description -->
// drag and drop of .scg files (on console) used "xload" and not "load"

fh = scf();
plot3d();
save(TMPDIR + "/foo.scg", "fh");
close(fh);

// go to TMPDIR with your file explorer
// select foo.scg and drag and drop on scilab console
// check that plot3d was restored


