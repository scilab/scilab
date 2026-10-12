// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 12548 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/12548
//
// <-- Short Description -->
// Duplicated code in xmltoformat

function a = bug_12548(b, c, d)
  a = 0;
endfunction

test_bug_12548 = "bug_12548";
pathDest = fullfile(TMPDIR, test_bug_12548, "help", getlanguage());
mkdir(pathDest);

mputl(help_skeleton("bug_12548"), fullfile(pathDest, "bug_12548.xml"));

assert_checktrue(execstr("xmltoformat(""html"",pathDest, ""bug help"")","errcatch")==0);
