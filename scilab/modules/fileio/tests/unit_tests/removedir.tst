// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// ============================================================================
// Unitary tests for removedir & rmdir functions
// ============================================================================
warning('off');
baseDir=TMPDIR+"/plop";
mkdir(baseDir);
mkdir(baseDir+"/aze");
mkdir(baseDir+"/aze/aze");
mkdir(baseDir+"/aze/aze/qsdq");
res=removedir(baseDir);
if res <> %t then pause,end
if isdir(baseDir) <> %f then pause,end

baseDir=TMPDIR+"/plop";
mkdir(baseDir);
mkdir(baseDir+"/aze");
mkdir(baseDir+"/aze/aze");
mkdir(baseDir+"/aze/aze/qsdq");
res=rmdir(baseDir);
if res <> 1 then pause,end
if isdir(baseDir) <> %f then pause,end
