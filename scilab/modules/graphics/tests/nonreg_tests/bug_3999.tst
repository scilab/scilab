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
// 

x = 1:10;
plot(x);
xbasc();

// check that there only an axes below the current figure
fig = gcf();
// only axes
if (size(fig.children) <> [1,1]) then pause; end
// no children below axes
if (fig.children(1).children <> []) then pause; end

// same with other version of xbasc
plot(x);
plot3d(x,x,cos(x)'*sin(x));

// erase all figure with index from 0 to 3
xbasc(0:3);

// we should have only empty figures
for i=0:3,
  fig = get_figure_handle(i);
  if (size(fig.children) <> [1,1]) then pause; end
  // no children below axes
  if (fig.children(1).children <> []) then pause; end
end