// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7855 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7855
//
// <-- Short Description -->
// Can't change the event function while the event handler is set to on.

function g(win,x,y,ibut)
    disp(ibut+1);
endfunction

function h(win,x,y,ibut)
    disp(ibut);
endfunction

f=gcf();
f.event_handler_enable = "on";

f.event_handler = 'g';
//Drag your cursor in the window and
//see the zeros displayed.

f.event_handler = 'h';
//Drag your cursor in the window and
//see the -1 displayed.
