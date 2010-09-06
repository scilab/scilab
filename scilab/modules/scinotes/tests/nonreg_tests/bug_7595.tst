// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7595 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7595
//
// <-- Short Description -->
// In SciNotes, a string was broken if '..' was encountered.

// Open SciNotes
// disp("Hello .. World");
// disp("Hello .. //comment");

// In the first case "Hello .. World" must be colorized as a string
// In the second the comment must be green and "Hello " as a string
