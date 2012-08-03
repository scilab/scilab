// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8529 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8529
//
// <-- Short Description -->
// mkdir did not create in the same instruction one folder and one subfolder.
//

cd;
test_7_dir  =  TMPDIR+"/mkdir_test_7/mkdir_test_7";
status_7    =  mkdir(test_7_dir);
if status_7 <> 1 then pause,end
if ~isdir(TMPDIR+"/mkdir_test_7/mkdir_test_7") then pause, end

cd TMPDIR;
if mkdir('essai1/tmp') <> 1 then pause,end
if ~isdir('essai1/tmp') then pause, end

if mkdir(TMPDIR + '/a/b/c/d/e/f') <> 1 then pause,end
if ~isdir(TMPDIR + '/a/b/c/d/e/f') then pause, end

