// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 9301 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9301
//
// <-- Short Description -->
// Exception when one tryed to paste a column to replace a column selection

mputl(['a(1, :) = [1 2 3];';
       'a(2, :) = [1 2 3];';
       'a(3, :) = [1 2 3];'], TMPDIR + '/bug_9301.sce')

scinotes(TMPDIR + '/bug_9301.sce');

// Select the column of 1 with <CTRL>+<MOUSE DRAGGED> and copy with <CTRL>+C
// Select the column of 2
// <CTRL>+V to paste, should replace each 2 by 1\n1\n1
// <CTRL>+Z to cancel, select the column of 2 and <CTRL+SHIFT>+V should replace
// the 2s with 1s. 
