// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->
// <-- Non-regression test for bug 12481 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12481
//
// <-- Short Description -->
// xlabel could not be used with Scilab property names
//

xlabel("foo","font_size", 3, "font_style", 9, "font_foreground", 5);

