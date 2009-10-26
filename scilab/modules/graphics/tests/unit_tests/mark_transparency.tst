// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - pierre.lando@scilab.org
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
//

// <-- Short Description -->
//   Add transparency to markdrawing

// Try this :

plot(1:10,sin(1:10))
e=gce();
e.children(1).mark_mode='on'
e.children(1).mark_style=9
e.children(1).mark_size_unit='tabulated'
e.children(1).mark_size=4
e.children(1).mark_background=0

// check that marks are transparent.

if e.children(1).mark_background<>0 then
  pause;
end
