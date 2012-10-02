// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3963 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3963
//
// <-- Short Description -->
// Destroying wait bars or progression bars crashes Scilab.
// 

// create a waitbar and a progression bar and close them
win1=waitbar('This is an example');
win2=progressionbar('HELLO!');
delete([win1,win2]);

