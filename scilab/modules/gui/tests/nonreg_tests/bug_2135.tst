// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 2135 -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2135
//
// <-- Short Description -->
// Some right press where lost

// Launch scilab
// Launch this script

lines(0);
gcf();
while %t
  x = xgetmouse();
  if x(3)==2
    disp("Got right button pressed...")
    break;
  else 
    disp(x);
  end
end

// Now try to press right mouse button.
// Do not release it
// Do not move your mouse
// Press <> Click

