// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Clement DAVID - ESI Group
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16714 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16714
//
// <-- Short Description -->
// The fullpath of a file is on to the current directory rather than being on
// the passed path.
//

p  = pwd() + filesep() + "nonexisting_file.sce";
assert_checkequal(p, fullpath(p));

directory = "foo";
mkdir(directory);
cd(directory);
assert_checkequal(p, fullpath(p));

cd("..");
removedir(directory);
