// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 1829 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1829
//
// <-- Short Description -->
// The clipping doesn't work with the strings of characters
//
 
a = gca();
a.data_bounds = [1,1;10,10];
a.box = "on";
for i = -1:11,
  for j = -1:11,
    xstringb(i,j,'Scilab',0,0);
    e = gce();
    e.clip_state = "clipgrf";
  end
end

// check that strings outside the axes are clipped
