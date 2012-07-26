// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11420 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11420
//
// <-- Short Description -->
// When using the event_handler of the default figure, events are not trapped.

function h(win,x,y,ibut)
    disp(ibut);
endfunction

set(gdf(),'event_handler','h');
set(gdf(),'event_handler_enable','on');

f=gcf();
//Drag your cursor in the window and
//see the -1 displayed.
