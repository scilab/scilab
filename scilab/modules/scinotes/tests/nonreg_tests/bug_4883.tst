// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan SIMON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 4883 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4883
//
// <-- Short Description -->
// "Menu→Document→Auto Indent" preference is not saved.


//scinotes()
//uncheck "Menu→Document→Auto Indent"
//close scinotes
//scinotes()
//=> "Menu→Document→Auto Indent" should stay unchecked

