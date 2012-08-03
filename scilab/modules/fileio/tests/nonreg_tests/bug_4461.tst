// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009 - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4461 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4461
//
// <-- Short Description -->
// prompt('TEST Prompt->') not managed by a diary


// <-- INTERACTIVE TEST -->

// with current diary we can not do many diaries

diary(TMPDIR+'/testdiary.txt')
prompt('- TEST Prompt ->')
1+1
disp(1:10)
diary(0)


// opens TMPDIR+'/testdiary.txt' with a text editor and checks that
// -->prompt('TEST Prompt->')
// TEST Prompt->1+1


