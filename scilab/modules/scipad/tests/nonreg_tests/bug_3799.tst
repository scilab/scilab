// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 3799 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3799
//
// <-- Short Description -->
//    Scipad crahes

// 1 - Start Scilab

// 2 - Start Scipad
// fig=figure("Position",[10,10,620,800]);
// fig.event_handler="follow";
// fig.event_handler_enable="on";                          
// function y=f(x); y=x; endfunction
// x=linspace(0,2,1000);
// fplot2d(x,f);
// [b,x,y]=xclick();
// rep=[x,y,-1];
// while rep(3)==-1 do
//   rep=xgetmouse();
// end

// 4 - CTRL+L

// 5 - Check that Scilab and Scipad are alive
