// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 12976 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12976
//
// <-- Short Description -->
// getURL() returns a file name instead of a file path

curdir = pwd();
cd(TMPDIR);

filepath = getURL("http://www.scilab.org");
assert_checkequal(filepath, fullfile(TMPDIR, "index.html"));
deletefile(filepath);

filepath = getURL("http://www.scilab.org", fullfile(TMPDIR, "scilab_homepage.html"));
assert_checkequal(filepath, fullfile(TMPDIR, "scilab_homepage.html"));
deletefile(filepath);

cd(curdir);
