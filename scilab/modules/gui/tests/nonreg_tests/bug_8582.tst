*// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8582 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8582
//
// <-- Short Description -->
// Under Linux & Mac OS the waitbar window was too small and we could not see the bar inside it.

// Exec the following example
winId=waitbar('This is an example');
realtimeinit(0.3);
for j=0:0.1:1,
  realtime(3*j);
  waitbar(j,winId);
end

// Check you see the bar

// Close the waitbar
delete(winId);