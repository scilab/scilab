// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5004 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5004
//
// <-- Short Description -->
//    Opening a file with accented characters with text-editor crashes Scilab very frequently.


//(With a terminal encoded into UTF-8)

//[$SHELL] echo 'éééééé' > with_accent.txt
//[$SHELL] echo 'eeeeee' > without_accent.txt

//--> editor("without_accent.txt"); // Open/Close editor ten times → Never crash
//--> editor("with_accent.txt"); // Open/Close editor ten times








