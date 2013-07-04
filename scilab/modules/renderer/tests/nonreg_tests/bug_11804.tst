// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 11804 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11804
//
// <-- Short Description -->
// Untranslated prompt messages

assert_checktrue(getlanguage()~='en_US');
f=figure("figure_name","bug_11804");
plot();
// Click button "zoom area"
// Check the messages at the bootom of figure
// are in the selected language
