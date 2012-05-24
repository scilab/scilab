// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// ============================================================================
// Unitary tests for removedir function
// ============================================================================

baseDir=pathconvert(TMPDIR+"/test_removedir",%F,%F);

mkdir(baseDir);
mkdir(pathconvert(baseDir+"/test_removedir_1",%F,%F));
mkdir(pathconvert(baseDir+"/test_removedir_1/test_removedir_2",%F,%F));
mkdir(pathconvert(baseDir+"/test_removedir_1/test_removedir_2/test_removedir_3",%F,%F));

if ~isdir(pathconvert(baseDir+"/test_removedir_1/test_removedir_2/test_removedir_3",%F,%F)) then pause,end

res=removedir(baseDir);

if ~res then pause,end
if isdir(baseDir) then pause,end
