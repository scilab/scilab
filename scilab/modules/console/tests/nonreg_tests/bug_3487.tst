// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3487 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3487
//
// <-- Short Description -->
//In nw & nwni modes:
//-->cd modules/umfpack/s[TAB]
//sci_gateway src
//type r+[TAB]
//-->cd modules/umfpack/sr?
//=> a strange char is added
//Other example:
//-->cd modules/ou[TAB]
//to get "output_stream"
//but I get:
//"modules/oueam"


// <-- INTERACTIVE TEST -->
// On Windows
// launch a shell (cmd.exe)
// go to in SCI/bin
// scilex.exe
// cd SCI
//-->cd modules/umfpack/s[TAB]
//-->cd modules/umfpack/sr[TAB]
