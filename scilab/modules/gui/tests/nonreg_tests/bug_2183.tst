// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2183 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2183
//
// <-- Short Description -->
//    The content typed into an 'edit' uicontrol cannot be retrieved if initially unset


f = figure(1);

// Create the uicontrol
h=uicontrol(f,"style","edit","position",[20 20 80 20])

// Now type something in the entry box

get(h,"string") // This line sould not return an error

