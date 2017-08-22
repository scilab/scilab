// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8180 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8180
//
// <-- Short Description -->
// Editing parameters of the EXPRESSION block changed the IEEE mode.

mymode = ieee();
ieee(2);
// * Drag and drop a User-defined => EXPRESSION block in a diagram
// * Double-clic on it to open its Parameters editor
// * Change nothing and just validate
ieee();
// The IEEE mode should still be 2
// Do the same after
ieee(1);
// ...

// Restore mymode
ieee(mymode);
