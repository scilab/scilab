// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 12615 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12615
//
// <-- Short Description -->
// Dead graphics windows after error in plot, bar or barh.

execstr("plot(1:10,1:10,""foo"");", "errcatch");
f=gcf();
assert_checkequal(f.immediate_drawing, "on");
delete(gcf())
execstr("bar([1 -3 5],0.5,""foo"");", "errcatch");
f=gcf();
assert_checkequal(f.immediate_drawing, "on");
delete(gcf())
execstr("barh([1 -3 5],0.5,""foo"");", "errcatch");
f=gcf();
assert_checkequal(f.immediate_drawing, "on");
delete(gcf())
