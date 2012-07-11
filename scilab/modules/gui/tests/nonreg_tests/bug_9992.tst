// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9992 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9992
//
// <-- Short Description -->
// gcbo not initialized in callbacks for uimenus in console

h0 = uimenu("parent", 0, "label", "level0"); 
h1 = uimenu("parent", h0, "label", "level1"); 
set(h1, "callback", "CB2()"); 
function CB2()
  disp("CB2");
  assert_checkequal(typeof(gcbo), "handle");
  assert_checkequal(size(gcbo), [1 1]);
endfunction

// Click on level0/level1 menu
// Check that "CB2" is displayed in the console
// Check that you do not have any error
