// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


// <-- TEST WITH SCINOTES -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 5327 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5327
//
// <-- Short Description -->
// When indenting a block, the selection area is shifted one line down with respect to the previously selected one.


editor()

// Write the following lines :

// for i=1:10
// if i>5, disp("Hi !"); end
// if (i==1)
//   disp("starting test");
// end
// end

// <CTRL+A>
// <CTRL+I>




