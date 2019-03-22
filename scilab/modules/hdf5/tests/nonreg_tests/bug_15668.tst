// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15668 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15668
//
// <-- Short Description -->
// save(filename) saves all predefined Scilab constants %e %pi etc.. (regression)


clear
x=1;
save(fullfile(TMPDIR,"session.sod"));
assert_checkequal(listvarinfile(fullfile(TMPDIR,"session.sod")),"x")