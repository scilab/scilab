// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
// <-- Non-regression test for bug 7214 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7214

xcos();
// put a SUPER_f block (User defined palette)
// open it
// close the main diagram
// close the super block diagram
// -> check that a saving popup appears without cancel button
// click No
// close Scilab
// -> check that no popup appears

