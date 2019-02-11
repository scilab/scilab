// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF --> 
//
// <-- Non-regression test for bug 14388 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14388
//
// <-- Short Description -->
// Addressing a cell component or clearing a cell array might crash Scilab

for i = 1:10
    clear c
    c(2,2) = {%pi}
    c{1,1}
    clear c
end
