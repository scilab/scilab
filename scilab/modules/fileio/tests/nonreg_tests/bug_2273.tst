// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2273 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2273
//
// <-- Short Description -->
// Scilab didn't manage UNC path on Windows

// <-- INTERACTIVE TEST -->

// creates a UNC path example \\MON-PC\TEST-UNC-PATH
// launch scilab from this UNC path
cd '\\MON-PC\TEST-UNC-PATH'
pwd
cd SCI
cd WSCI
chdir '\\MON-PC\TEST-UNC-PATH'