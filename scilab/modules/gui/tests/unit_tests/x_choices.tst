// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// unit tests for x_dialog function
// =============================================================================


// <-- INTERACTIVE TEST -->

// test 1
l1   = list("choice 1", 1, ["toggle c1","toggle c2","toggle c3"]);
l2   = list("choice 2", 2, ["toggle d1","toggle d2","toggle d3"]);
l3   = list("choice 3", 3, ["toggle e1","toggle e2"]);
rep  = x_choices("Toggle Menu",list(l1,l2,l3));

// test2
tog  = "toggle ";
tog  = tog(ones(1,500))+string(1:500);
l1   = list("choice 1", 1,tog);
l2   = list("choice 2", 2,["toggle d1","toggle d2", "toggle d3"]);
l3   = list("choice 3", 3,["toggle e1","toggle e2"]);
rep  = x_choices("Toggle Menu",list(l1,l2,l3));
