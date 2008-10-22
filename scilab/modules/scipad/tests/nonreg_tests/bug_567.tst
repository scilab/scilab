// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 567 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=567
//
// <-- Short Description -->
//    When I accidentally closed scilab while scipad editor was
//    open, scipad did not ask to save the changes. This is a
//    serious bug since all unsaved changes were lost. The best
//    solution might be to leave the editor window open as a
//    separate application.


disp("start scilab");
disp("start scipad");
disp("write something in scipad");
disp("don''t save!");
disp("quit scilab");
