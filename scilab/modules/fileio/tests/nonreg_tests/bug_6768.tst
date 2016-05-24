// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM MANDATORY -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 6768 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6768
//
// <-- Short Description -->
// input did not return good value when RETURN was pressed on Windows XP.
//

x = input("Press RETURN", "string");
if x <> "" then pause, end
