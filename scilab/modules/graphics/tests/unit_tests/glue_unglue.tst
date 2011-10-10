// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test glue/unglue capabilities

f = gcf();
a = gca();
xrect(-1,1,2,2);
xarc(-0.5,0.5,2,2,0,360*64);

assert_checkequal(a.children(1).type, "Arc")
assert_checkequal(a.children(2).type, "Rectangle")
e = gce();
assert_checkequal(e.type, "Arc")

arc_child=a.children(1);
rect_child=a.children(2);

glue([arc_child, rect_child]);

assert_checkequal(a.children(1).type, "Compound")
e = gce();
assert_checkequal(e.type, "Compound")
assert_checkequal(e.children(1).type, "Rectangle")
assert_checkequal(e.children(2).type, "Arc")


unglue(e);
assert_checkequal(a.children(1).type, "Rectangle")
assert_checkequal(a.children(2).type, "Arc")
e = gce();
assert_checkequal(e.type, "Axes")
