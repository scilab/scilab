// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2797 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2797
//
// <-- Short Description -->
//    After an error when plotting with the syntax "plot(x,f)", the following plots
//    are empty.

function r = f(x);
  r = 1 / x;
endfunction

x = linspace(-1, 1, 101);

curFig = gcf();
// just to be sure
curFig.immediate_drawing = "on";

// evaluation of f will fail
// it use to keep current figure in drawlater mode
//plot(x,f)

//the error will not be printed in the console
ierr = execstr('plot(x,f)','errcatch');

plot(x,x)

// check we are not in drawlater mode anymore
if (curFig.immediate_drawing <> "on") then pause; end


