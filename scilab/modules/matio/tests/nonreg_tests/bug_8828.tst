// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8828 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8828
//
// <-- Short Description -->
// "savematfile" could no more save a structure.

Astruct.str = "example";
Astruct.num = 123456789;
savematfile(TMPDIR + filesep() + "bug_8828.mat", "Astruct", "-v6");
