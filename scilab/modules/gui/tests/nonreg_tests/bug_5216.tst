// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 5216 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5216
//
// <-- Short Description -->
// In ATOMS GUI, it would be nice to be able to click on link or at least to copy/paste the URL.


h = uicontrol("style","text",..
	"string", "<a href=""http://www.scilab.org/"">Visit Scilab website...</a>",..
	"position",[20 20 200 200], ...
	"fontsize", 15)

// Click on the link and check that a web browser is opened