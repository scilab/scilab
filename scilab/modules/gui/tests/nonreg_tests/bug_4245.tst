// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 4245 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4245
//
// <-- Short Description -->
// The icon of the progressionbar is a java icon. It should it be a Scilab one instead.

// Enter:
winId=progressionbar('Do something');
// Check that the icon of the window open with a progressbar in it is a Scilab icon

// Now enter:
delete(winId);

// Enter:
winId=waitbar('This is an example');
// Check that the icon of the window open with a waitbar in it is a Scilab icon

// Now enter:
delete(winId);


