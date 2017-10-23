// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 15223 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15223
//
// <-- Short Description -->
// call crashs in some cases

c = [ ...
    "#include <stdio.h>";
    "";
    "void bug_15223(void)";
    "{";
    "    printf(""call without input/output\n"");";
    "}"]

f = fullfile(TMPDIR, "bug_15223.c");
mputl(c, f);
cd(TMPDIR);
libname = ilib_for_link("bug_15223", "bug_15223.c", [], "c");
exec loader.sce;
call("bug_15223");

ulink();

//if test finish without crash it is OK
