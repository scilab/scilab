// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->

ilib_verbose(0);

copyfile(fullfile(SCI, "modules/api_scilab/tests/nonreg_tests/bug_15121.c"), ..
    fullfile(TMPDIR, "bug_15121.c"));

cd(TMPDIR);
ilib_build("libbug_15121", ["bug_15121", "sci_bug_15121"], "bug_15121.c", ..
    [], "", "", "");
exec("loader.sce");

bug_15121(A=2);

ulink(0);