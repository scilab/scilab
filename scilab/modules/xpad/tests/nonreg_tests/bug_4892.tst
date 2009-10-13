// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4892 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4892
//
// <-- Short Description -->
// scilab editor under MacOSX :
// 
// Shortcuts are not managed at all. They should use the Command key ("Apple key").
// 
// For example :
// 
// Command-C  Copy selected item/text
// Command-N  New file
// 
// etc.


editor()
// Just type Command+N
