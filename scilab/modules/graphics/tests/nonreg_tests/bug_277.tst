// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 277 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=277
//
// <-- Short Description -->
//  Definition of a plot frame by plotframe (subwin argument) leads to messy coordinates.
//  Apparently a switch from real to pixel coords occurs

// create a new axes with data bounds [0,0;1000,1])
GrphFrame = [-.05   0.  .85  .5 ];
xMin = 0;
xMax = 1000;
yMin = 0;
yMax = 1;
plotframe([xMin yMin xMax yMax],[1,1,1,1],[%f,%t],["","",""],GrphFrame)

// select 4 points in the axes
points = locate(4,1);

// their coordinates must be within the data bounds
for i=1:4,
  if (points(1,i) > xMax ) then pause; end
  if (points(1,i) < xMin ) then pause; end
end


