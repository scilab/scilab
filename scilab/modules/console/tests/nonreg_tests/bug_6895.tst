// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6895 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6895
//
// <-- Short Description -->
// completion crashed in -NW mode on some linux.

// <-- INTERACTIVE TEST -->

// launch scilab -NW mode
// enter the following command in the scilab console:
// xml2jar('pt_BR')
// then go back to the '2', remove it and while the cursor is on the 'j', enter <TAB>
