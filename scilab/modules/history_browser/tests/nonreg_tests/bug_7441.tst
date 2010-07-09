// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 7441 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7441
//
// <-- Short Description -->
// history browser crashed if language was changed by user.

// on linux, start scilab in STD mode
// LANG=fr_FR bin/scilab
1+1
exit

// on linux, start scilab in STD mode
// LANG=en_US bin/scilab
23
exit

// restart scilab in STD mode
// if scilab did not crash, all is oky


// on windows,
setdefaultlanguage('en')
1+1
2
exit

// restart scilab in STD mode
setdefaultlanguage('fr')
1+1
2
exit

// restart scilab in STD mode
// if scilab did not crash, all is oky


