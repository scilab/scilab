// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3937 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3937
//
// <-- Short Description -->
// When we use x_choices, the size of the buttons depends on the largest string of the variable "title". So if the largest title is too long, the size of the buttons will also be too long, whatever the size of the string on these buttons.

option= list(["Very veryyyyyyyyyyyyyyyyyyyyyyy loooooooooooooooonnnnnnnngg label"], 1, ["Yes", "No"]);
choiceSettings=x_choices('Settings Menu', list(option));

// Check that the buttons width is smaller that the label width

// Resize the window & check that the buttons size does not change