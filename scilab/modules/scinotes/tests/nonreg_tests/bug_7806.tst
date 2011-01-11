// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7806 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7806
//
// <-- Short Description -->
// Blocks elseif...end and else...end were not highlighted.

tmpfile = TMPDIR + "/bug_7806.sci";
code = ["if a then", "b", "elseif c then", "d", "else", "e", "end"];
mputl(code, tmpfile);
scinotes(tmpfile);

// Put the mouse over "if", the block if...end must be highlighted
// Put the mouse over "elseif", the block elseif...end must be highlighted
// Put the mouse over "else", the block else...end must be highlighted
