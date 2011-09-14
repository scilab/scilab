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
// <-- Non-regression test for bug 6819 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6819
//
// <-- Short Description -->
// prompt() interpreted '<...>' as html

prompt('<1>')

// must display <1> !