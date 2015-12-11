// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 12971 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12971
//
// <-- Short Description -->
// getURL() downloaded file name is wrong

curdir = pwd();
cd(TMPDIR);

filePath = getURL("www.scilab.org");

expectedFilePath = fullfile(TMPDIR, "index.html");
assert_checkequal(filePath, expectedFilePath);
assert_checktrue(isfile(filePath));

deletefile(filePath);
cd(curdir);