// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7629 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7629
//
// <-- Short Description -->
// Double-clicking an entry in browsevar opens it in editvar; however, if the
// variable no longer exists it seems to be treated as something which cannot be
// edited.
//
// This then causes an error because Scilab tries to get the typeof() the
// variable, which does not exist.
//
// Error is not limited to "ans" as demonstrated with "x".
//
// This breaks the double-click to open, but editvar(nameAsString) still works.

a = 1;
browsevar();
clear a;
// Double-click on the "a" cell
