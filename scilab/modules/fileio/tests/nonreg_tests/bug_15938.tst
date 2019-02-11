// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- MACOSX ONLY -->//
// <-- Non-regression test for bug 15938 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=15938
//
// <-- Short Description -->
// fullpath() is broken on OSX

cd(TMPDIR)
path=fullfile("level1","level2")
assert_checkequal(fullpath(path),fullfile(TMPDIR,"level1","level2"))