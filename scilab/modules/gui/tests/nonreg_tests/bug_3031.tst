// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 3031 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3031
//
// <-- Short Description -->
//   uicontrol property names could be abbreviated in scilab 4.1.2, need to be extended in scilab 5

figure();

if execstr("uicontrol(""posi"",[10 10 100 100])", "errcatch")<>0 then pause; end

