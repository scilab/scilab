// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 7491 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7491
//
// <-- Short Description -->
// Lines broken with a .. were not correctly handled and caused
// a freeze of SciNotes.

// launch SciNotes
// just type: "Hello..
//             World..
//             How are you..
//             ?"
// SciNotes froze when the second point after World was typed
