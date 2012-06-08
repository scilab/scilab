// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 10637 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10637
//
// <-- Short Description -->
// xs2svg crashs when call with something else an figure handle

plot3d();
f = gcf();
err = execstr("xs2svg(f, TMPDIR + ""/Figure"", ""landscape"")", "errcatch");
assert_checkequal(err, 0);
a = gca();
err = execstr("xs2svg(a, TMPDIR + ""/Axes"", ""landscape"")", "errcatch");
assert_checktrue(err <> 0);
