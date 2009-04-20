// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4026 -->
//

// <-- Short Description -->
//
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4026
//

// <-- INTERACTIVE TEST -->
// <-- WINDOWS ONLY -->

// With the "unix" command under Windows, it is not possible to run an external program which has a long path name.

// put a executable in a long path name
// unix("C:/Program Files/OpenOffice.org 3/program/scalc.exe")
// scalc must be launched
