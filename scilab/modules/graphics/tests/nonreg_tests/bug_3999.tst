// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3999 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3999
//
// <-- Short Description -->
// xbasc() doesn't clear the current graphics window.
// xbasc replaced by clf
//

x = 1:10;
plot(x);
clf();

// Check that there is only one axe below the current figure
fig = gcf();
// Only axes
assert_checkequal(size(fig.children), [1,1]);
// No children below axes
assert_checkequal(fig.children(1).children, []);

// Same with other version of xbasc
plot(x);
plot3d(x,x,cos(x)'*sin(x));

// Erase all figures with index from 0 to 3 even if some of them do not exist
clf(0:3);

// Current figure must be empty
fig = get_figure_handle(0);
assert_checkequal(size(fig.children), [1,1]); // Default axes
// No children below axes
assert_checkequal(fig.children(1).children, []);
