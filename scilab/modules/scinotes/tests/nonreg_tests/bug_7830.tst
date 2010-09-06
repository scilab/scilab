// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7830 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7830
//
// <-- Short Description -->
// There was a problem when opening a file at a line number.

editor(get_function_path("prettyprint"), 123);

// the file prettyprint must be opened at line 123 and it must be highlighted.
