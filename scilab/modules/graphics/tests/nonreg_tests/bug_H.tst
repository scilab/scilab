// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug H -->
//
// <-- OLD BUG -->
//
// Une option de contour2d a ete oubliee dans la version 2.7

result = execstr("contour2d(1:10,1:10,rand(10,10),[0.1 0.5 0.9])","errcatch","n");
xdel();
if result <> 0 then pause,end
