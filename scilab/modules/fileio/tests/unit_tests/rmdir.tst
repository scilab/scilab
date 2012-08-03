// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// ============================================================================
// Unitary tests for rmdir function
// ============================================================================

baseDir=pathconvert(TMPDIR+"/test_rmdir",%F,%F);

mkdir(baseDir);
mkdir(pathconvert(baseDir+"/test_rmdir_1",%F,%F));
mkdir(pathconvert(baseDir+"/test_rmdir_1/test_rmdir_2",%F,%F));
mkdir(pathconvert(baseDir+"/test_rmdir_1/test_rmdir_2/test_rmdir_3",%F,%F));

if ~isdir(pathconvert(baseDir+"/test_rmdir_1/test_rmdir_2/test_rmdir_3",%F,%F)) then pause,end

res=rmdir(baseDir,'s');

if ~res then pause,end
if isdir(baseDir) then pause,end
