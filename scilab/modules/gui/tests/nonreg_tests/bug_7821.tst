// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7821 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7821
//
// <-- Short Description -->
// In a messagebox, an exception was thrown when clicking on a mal-formed url.

messagebox("<html><body><a href=""this file doesn''t exist"">this file doesn''t exist</a></body></html>")

// Click on the hyperlink

// A messagebox is opened to alert that the url cannot be opened
