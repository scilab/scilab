// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7388 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7388
//
// <-- Short Description -->
// A java exception is thrown when we click on a menu made with uimenu.

uimenu(gcf(),"foo","Foo")

//Click on the menu Foo

// Check that no exception occurs