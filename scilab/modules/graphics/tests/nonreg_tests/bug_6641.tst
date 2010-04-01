// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO - Lando Pierre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6641 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6641
//
// <-- Short Description -->
// hist3d did not handle correctly optionals arguments.
//
// run this :

hist3d(10*rand(10,10),5,5,"x@u@p",[1,1,4],[-10,10,-10,10,-10,10]);


// legends should be "x", "u" and "p", and the bounding box [-10, 10] ^ 3
