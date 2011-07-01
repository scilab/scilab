// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 9358 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9358
//
// <-- Short Description -->
// A simple quoted string was not always correctly convert into double quoted one.

mputl("''''''i''''''''m done dude''''''", TMPDIR + '/bug_9358.sce')
scinotes(TMPDIR + '/bug_9358.sce');

// Menu Edition
// Submenu "Single to double quote strings"
// the string must be transformed in "''i''''m done dude''"