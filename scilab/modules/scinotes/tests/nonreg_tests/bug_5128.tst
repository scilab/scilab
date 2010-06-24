// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5128 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5128
//
// <-- Short Description -->
//   Modified file saved as the other file when there are 2 files loaded in the editor.


// Load file "toto1.sce" in the editor
// Load file "toto2.sce" in the editor
// Modify "toto1.sce"
// Quit the editor and asks yes for saving "toto1.sce"
// Have a Look to the content of both files
