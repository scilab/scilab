// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Francois Vogel
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3733 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3733
//
// <-- Short Description -->
//  Scilab gets stuck (endless loop) when entering:
//  mprintf("%3.0f equals two \r",2)

// <-- INTERACTIVE TEST -->
//
//
// paste the lines below in Scilab: it must not hang

// used to hang Scilab
mprintf("%3.0f equals two \r",2)

for i=1:30
  mprintf("%3.0f is a number \r",i)
  for i=1:10000;end
end
mprintf("Game over.......\n")

