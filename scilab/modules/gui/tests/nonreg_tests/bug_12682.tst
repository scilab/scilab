// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 12682 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12682
//
// <-- Short Description -->
// Key events were disabled after zooming.

function MyEvents(fig_id,x,y,ibut)
    if ibut>0 then disp(ibut);end
endfunction

fig=gcf();
clf();
fig.event_handler="MyEvents";
t=linspace(0,20*%pi,10000);
plot(t,sin(t))
fig.event_handler_enable="on";

// Hit A key to check that something is printed
// Now zoom in using zoom button
// Hit A key and check that something is printed