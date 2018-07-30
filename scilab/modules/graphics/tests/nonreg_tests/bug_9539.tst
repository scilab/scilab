// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 9539 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9539
//
// <-- Short Description -->
// Nsize = 1600;
// [xc,yc]=contour2di(1:Nsize,1:Nsize,rand(Nsize,Nsize,'uniform'),[0.5 0.5]);
// crashed Scilab

rand('seed',getdate('s'));
Nsize = 1600;
comm = "[xc,yc]=contour2di(1:Nsize,1:Nsize,rand(Nsize,Nsize,''uniform''),[0.5 0.5])";
assert_checktrue(execstr(comm, "errcatch")==0);
