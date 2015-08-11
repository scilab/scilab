// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Allan.Simon@scilab.org
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH EDITOR -->
//
// <-- Non-regression test for bug 4881 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4881
//
// <-- Short Description -->
//    In the "Menu→Search→find/replace" dialog window, check the radio box "Selected lines" has no effect : The whole file is always processed.

editor SCI/modules/atoms/macros/atomsInstall.sci

//"Menu→Search→Find/Replace"
//Find    : " "
//Replace : ""
//Check the "Selected lines" box
//Select the 3 first lines

//Press the button "Replace All"

//=> All spaces are erased.

