// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4174 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4174
//
// <-- Short Description -->
// Graphic export : The xs2xxx (graphic export) functions don't support paths that start with a tilde.
// 

scf(10); 
plot3d();
xs2png(10,'~/bug_path.png')
                
// check if the exported file is in the user's home(/home/user) directory.


