// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM MANDATORY -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- NO CHECK ERROR OUTPUT --> 
//
// <-- Non-regression test for bug 8505 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8505
//
// <-- Short Description -->
// help was not generated if there was a path name with some spaces.

function a = bug_8505(b, c, d)
  a = 0;
endfunction

dir_with_space = "directory with space";
pathDest = TMPDIR + filesep() + dir_with_space + filesep();
mkdir(TMPDIR, dir_with_space);

mputl(help_skeleton("bug_8505"), pathDest + "bug_8505.xml");

r = xmltojar(pathDest, "bug help");
if ~isfile(r) then pause, end

