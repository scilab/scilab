// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 6582 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6582
//
// <-- Short Description -->
// handle copy does not keep foreground property



xstring(1/2,1/2,'foo');e1=gce();
e1.font_foreground=5;
e1.font_style=9;

e2=copy(e1);
e2.data=[0.75,1/2];
assert_checkequal(e2.font_foreground, e1.font_foreground);