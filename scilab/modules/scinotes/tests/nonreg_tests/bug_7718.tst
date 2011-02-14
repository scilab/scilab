// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7718 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7718
//
// <-- Short Description -->
// There was a bad detection of a function when the list of args was broken.

// In SciNotes:
// function foo(a,..
//              b,..
//              c)
// endfunction

// a,b and c should be colorized (maroon+bold) as function's arguments.
