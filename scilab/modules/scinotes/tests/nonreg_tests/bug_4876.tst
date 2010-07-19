// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Allan.Simon@scilab.org
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH EDITOR -->
//
// <-- Non-regression test for bug 4876 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4876
//
// <-- Short Description -->
//    The cancel button of the "Goto Line" window doesn't work. The original position is forgotten.

editor SCI/modules/atoms/macros/atomsInstall.sci
<ctlr+G>
// Enter a line number, it doesn't matter what.
// Press the cancel buttonm, it doesn't return to original position

