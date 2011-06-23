// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// Checks that this test is skipped because it is interactive.
l1  = list("choice 1",1,["toggle c1","toggle c2","toggle c3"]);
l2  = list("choice 2",2,["toggle d1","toggle d2","toggle d3"]);
l3  = list("choice 3",3,["toggle e1","toggle e2"]);
rep = x_choices("Toggle Menu",list(l1,l2,l3))
