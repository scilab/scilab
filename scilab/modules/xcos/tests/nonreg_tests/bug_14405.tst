// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Nicolas Carrez
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14405 -->
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/14405
//
// <-- Short Description -->
// xcosPalAdd doesn't work

loadXcosLibs();
pal = xcosPal();
pal = xcosPalAddBlock(pal, "SUM_f");
pal = xcosPalAddBlock(pal, "BIGSOM_f");
assert_checktrue(xcosPalAdd(pal, "my Summation blocks"));

// Blocks without an existing style have their image path validated. On
// Windows these paths contain backslashes; failures go to the error output.
MYPALBLK = BIGSOM_f;
scs_m = BIGSOM_f("define");
scs_m.gui = "MYPALBLK";
pal = xcosPal("default style");
pal = xcosPalAddBlock(pal, scs_m);
assert_checktrue(xcosPalAdd(pal));

// image path with a space
d = fullfile(TMPDIR, "dir with space");
mkdir(d);
img = fullfile(d, "MYPALBLK2.svg");
copyfile(fullfile(TMPDIR, "MYPALBLK.svg"), img);
MYPALBLK2 = BIGSOM_f;
scs_m.gui = "MYPALBLK2";
pal = xcosPal("path with space");
pal = xcosPalAddBlock(pal, scs_m, [], img);
assert_checktrue(xcosPalAdd(pal));
