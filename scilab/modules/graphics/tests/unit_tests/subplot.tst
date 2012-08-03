// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// test subplot()

// Check each current figure property
f = gcf();
a1 = gca();

// Assert axes fill all figure
assert_checkequal(a1.axes_bounds, [0 0 1 1]);
// Assert figure only has 1 child
assert_checkequal(size(f.children), [1 1]);
assert_checkequal(f.children(1).type, "Axes");

subplot(10,10,1);
a2 = gca();
// Assert figure has still one child
assert_checkequal(size(f.children), [1 1]);
assert_checkequal(a2.axes_bounds, [0 0 0.1 0.1]);
// a2 == a1: simply check on axes bounds
assert_checkequal(a1.axes_bounds, [0 0 0.1 0.1]);

subplot(10,10,100);
a3 = gca();
// Assert figure has two children now
assert_checkequal(size(f.children), [2 1]);
assert_checkequal(a1.axes_bounds, [0 0 0.1 0.1]);
assert_checkequal(a3.axes_bounds, [0.9 0.9 0.1 0.1]);

subplot(10,10,10);
a4 = gca();
// Assert figure has three children now
assert_checkequal(size(f.children), [3 1]);
assert_checkequal(a1.axes_bounds, [0 0 0.1 0.1]);
assert_checkequal(a3.axes_bounds, [0.9 0.9 0.1 0.1]);
assert_checkequal(a4.axes_bounds, [0.9 0 0.1 0.1]);

subplot(10,10,91);
a5 = gca();
// Assert figure has four children now
assert_checkequal(size(f.children), [4 1]);
assert_checkequal(a1.axes_bounds, [0 0 0.1 0.1]);
assert_checkequal(a3.axes_bounds, [0.9 0.9 0.1 0.1]);
assert_checkequal(a4.axes_bounds, [0.9 0 0.1 0.1]);
assert_checkequal(a5.axes_bounds, [0 0.9 0.1 0.1]);
