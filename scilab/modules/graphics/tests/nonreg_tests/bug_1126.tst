// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1126 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1126
//
// <-- Short Description -->
// GIF driver  don't take into account the size of the window.


// create a figure
x = [0,1];
z = [0 0.5;0.5 1];

driver("GIF"); 
xinit('toto.gif')

f = gcf();
f.figure_size = [800,600];
f.color_map = [jetcolormap(64);[0.9 0.9 0.9]];
f.background = 65;
   
colorbar(0,1,colminmax=[1,64])
Sgrayplot(x,x,z,colminmax=[1,64])

xtitle("new mode is used: 800 x 600 picture ?")

// export it to gif
xend();
driver("Rec")

// Then open toto.gif in a image viewer. 


