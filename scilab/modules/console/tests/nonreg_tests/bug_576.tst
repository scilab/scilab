// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 576 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=576
//
// <-- Short Description -->
//    input() function of scilab cvs doesn't work like scilab 2.7.x
//    when the output argument is empty, the output is not detected
//    as an empty result ?

disp("press enter");
chemin=input('?','s');
if isempty(chemin) then pause,end
