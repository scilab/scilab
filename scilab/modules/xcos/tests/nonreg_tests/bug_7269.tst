// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 7269 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7269
//
// <-- Short Description -->
// When adding a non-existing user-custom block on a diagram, an exception is
// thrown.
//


// Adding a user custom block
pal = xcosPal("My pal");
scs_m = SUM_f("define");
blockPath = TMPDIR + "/SUM_f.sod";
save(blockPath, "scs_m"),

pal = xcosPalAddBlock(pal, blockPath);
xcosPalAdd(pal);
deletefile(blockPath);

xcos();
// Open "My pal" on the palette manager
// try to drag and drop the SUM_f block
// check that no exception is thrown

