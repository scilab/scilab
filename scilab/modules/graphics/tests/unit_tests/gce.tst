// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test gce behaviour

f = gcf();
a = gca();
e = gce();

assert_checkequal(e, a)

plot(1:10);

e = gce();
assert_checkequal(e.type, "Compound");
assert_checkequal(e.parent.type, "Axes");
assert_checkequal(e.children(1).type, "Polyline");

delete(e);

e = gce();

assert_checkequal(e.type, "Axes");
assert_checkequal(e.parent.type, "Figure");
assert_checkequal(e.children, []);