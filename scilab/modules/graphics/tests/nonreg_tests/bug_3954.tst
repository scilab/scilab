// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 394 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3954
//
// <-- Short Description -->
// Graphic demos launched using functions without arguments (such as plot3d()) may modify variables.
// 

f = gdf();
backupF = f;
x = 10;
backupX = x;
plot3d();
if (x <> backupX) then pause; end
if (f <> backupF) then pause; end
