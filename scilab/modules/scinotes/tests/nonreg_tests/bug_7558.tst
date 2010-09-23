// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7558 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7558
//
// <-- Short Description -->
// Global variables were reached before local ones.

// In Scilab's console :
c = 1;
// launch SciNotes
// just type:
// function toto(c,v)\n
// end

// the local var 'c' should be bold.