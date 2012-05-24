// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3529 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3529
//
// <-- Short Description -->
// rmdir() change directory permissions instead of deleting them.
wm = warning('query');
warning('off');
baseDir=TMPDIR+"/plop";
mkdir(baseDir);
mkdir(baseDir+"/aze");
mkdir(baseDir+"/aze/aze");
mkdir(baseDir+"/aze/aze/qsdq");
res=rmdir(baseDir,'s');
if res <> 1 then pause,end
if isdir(baseDir) <> %f then pause,end
