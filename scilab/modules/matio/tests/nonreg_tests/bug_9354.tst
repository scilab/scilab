// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9354 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9354
//
// <-- Short Description -->
// "savematfile" could not save imbricated structures.

// Try to save the structure
V.a.a = 1;
V.a.b = 2;
V.b.a = 3;
savematfile(TMPDIR + filesep() + "bug_9354.mat", "V", "-v7")

// Try to reload the structure
refV = V;
clear V
loadmatfile(TMPDIR + filesep() + "bug_9354.mat");

// Compare saved and read variables
assert_checkequal(V, refV);
