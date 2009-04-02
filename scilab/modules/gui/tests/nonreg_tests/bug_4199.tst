// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

//
// <-- Non-regression test for bug 4199 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4199
//
// <-- Short Description -->
//    Findobj does not find figures

fg = scf();
fg.tag = "bug_4199";

if isempty(findobj("tag","bug_4199")) then pause;end
