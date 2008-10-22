// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 1779 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1779
//
// <-- Short Description -->
//    Wrong uicontrol values retrieved when plotting

// All values displayed in the console must be 0 except the last one
// Check the checkbox 'Stop' to stop the program


f = figure(1);
hstop = uicontrol(f,"style","checkbox","string","Stop","min",0,"max",1,"value",0,"position",[20 20 50 50]);

scf();
stop=0;
while stop<0.5
 stop=get(hstop,"value");
 mprintf("%d ",stop)
 clf;
 Matplot(rand(50,50)*32)
end
