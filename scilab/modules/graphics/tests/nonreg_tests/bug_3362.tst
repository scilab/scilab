// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge Steer <Serge.Steer@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3362 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3362
//
// <-- Short Description -->
//    problem with incorrect legend in plot2d

t=(1:10)';
assert_checktrue(execstr("plot2d(t,t.^2,leg=''s@'')",'errcatch') == 0);
assert_checktrue(execstr("plot2d(t,t.^2,leg=''s@s1'')",'errcatch') == 0);
assert_checktrue(execstr("plot2d(t,[t.^2 t.^3],leg=''s'')",'errcatch') == 0);
