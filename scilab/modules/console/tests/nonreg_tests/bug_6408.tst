// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
// <-- TEST WITH CONSOLE -->
//
// <-- Non-regression test for bug 6408 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6408
//
// <-- Short Description -->
// prompt() removed trailing whites.

prompt('abcd               ')

// the whites must not be removed