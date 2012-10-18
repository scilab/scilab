// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 7953 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7953
//
// <-- Short Description -->
// No error message were printed when the diagram is invalid.

try
	xcos(mlist("diagram", "plop"));
catch, end
[str, n] = lasterror();
if n <> 999 then pause, end
if strstr(str, "xcos:") <> str pause, end

