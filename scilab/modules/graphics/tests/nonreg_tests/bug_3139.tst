// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3139 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3139
//
// <-- Short Description -->
// In log mode, vertical lines of the grid (just above the tic labels) are missing.

plot([0.01,100],[1,10]);
a           = gca();
a.log_flags = 'lnn';
a.grid      = [12,-1];

// check that all the grid line are drawn
// check that the label is in the window
rect = stringbox(a.title);

// convert it to pixel coordinates
for i = 1:4
  [pixPos(i,1), pixPos(i,2)] = xchange(rect(1,i),rect(2,i), "f2i");
end

// get canvas size
fig = gcf();
axesSize = fig.axes_size;

for i = 1:4
  if (pixPos(i,1) < 0 | pixPos(i,1) > axesSize(1)) then pause; end
  if (pixPos(i,2) < 0 | pixPos(i,2) > axesSize(2)) then pause; end
end

