// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - pierre.lando@scilab.org
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 4964 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4964
//
// <-- Short Description -->
//   set or get 'format_n' property from an axes crash scilab.


plot2d(1:10,1:10,1,strf="020")
drawaxis(x=2:7,y=4,dir="u",tics="v");
e=gce();
assert_checkequal(e.format_n, "");
// Check that the display does not crash
e.format_n = "%0.0fcm"
assert_checkequal(e.format_n, "%0.0fcm");
assert_checkequal(e.tics_labels, string(2:7)+"cm");


