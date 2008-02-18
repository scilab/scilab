//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Demonstrate animation based on the evolution of a 3D surface
// ============================================================================

curFig = gcf();clf();  // erase window
xselect(); //raise the graphic window

//turn off toolbar
//----------------
id=curFig.figure_id;
tb=toolbar(id); //preserve setting
toolbar(id,"off");

// set double buffer mode to avoid blinking animation
//---------------------------------------------------
pix = curFig.pixmap; //preserve old setting
curFig.pixmap = "on";

// set a new colormap 
//-------------------
cmap= curFig.color_map; //preserve old setting
curFig.color_map = jetcolormap(64);

//The initial surface definition 
//----------------------
x=linspace(-%pi,%pi,50);
y=x;
Z=sin(x)'*cos(y);

//Creates and set graphical entities which represent the surface
//--------------------------------------------------------------
plot3d1(x,y,Z,35,45,' ');
s=gce(); //the handle on the surface
s.color_flag=1 ; //assign facet color according to Z value
title("evolution of a 3d surface","fontsize",3)

I=20:-1:1;
realtimeinit(0.1);;//set time step (0.1 seconds)  and date reference
for i=1:max(size(I))
  realtime(i) //wait till date 0.1*i seconds
  s.data.z = (sin((I(i)/10)*x)'*cos((I(i)/10)*y))';
  show_pixmap();//send  buffer to screen
end
	

// Reset initial properties
//--------------------------------
toolbar(id,tb);
curFig.pixmap = pix;

