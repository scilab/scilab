// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 10116 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10116
//
// <-- Short Description -->
// for h = vector_of_handles, .., end  was not supported

plot()
f = gcf();
assert_checktrue(execstr("for c = f.children, end", "errcatch")==0);
