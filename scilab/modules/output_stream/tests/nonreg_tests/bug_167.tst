// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 167 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=167
//
// <-- Short Description -->
//    sprintf error

if MSDOS then
	if sprintf("%e", 1e-10) <> "1.000000e-010" then pause; end
else
	if sprintf("%e", 1e-10) <> "1.000000e-10" then pause; end
end
