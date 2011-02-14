// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 7110 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7110
//
// <-- Short Description -->
// Insert a struct in a handle will overwrite it.

a = gca();
if typeof(a) @= "handle" then pause, end
a.user_data = struct();
if typeof(a) @= "handle" then pause, end
if a.user_data @= struct() then pause, end
