// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 8505 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/8505
//
// <-- Short Description -->
// help was not generated if there was a path name with some spaces.

function a = bug_8505(b, c, d)
  a = 0;
endfunction

dir_with_space = "directory with space";
pathDest = fullfile(TMPDIR, dir_with_space, "help", getlanguage());
mkdir(pathDest);

mputl(help_skeleton("bug_8505"), fullfile(pathDest, "bug_8505.xml"));

r = xmltohtml(pathDest, "bug help");
assert_checktrue(isfile(r));
