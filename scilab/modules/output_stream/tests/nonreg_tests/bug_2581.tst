// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2581 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2581
//
// <-- Short Description -->
//    mprintf is broken

colors=["red";"green";"blue";"pink";"black"];
RGB=[1 0 0;0 1 0;0 0 1;1 0.75 0.75;0 0 0];

if execstr("mprintf(""%d\t%s\t%f\t%f\t%f\n"",(1:5)'',colors,RGB);", "errcatch")<>0 then pause; end
