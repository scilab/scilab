//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Demonstrate animation based on the evolution of a 3d curve
// =============================================================================

curFig = gcf();clf();  // erase window
xselect(); //raise the graphic window
mode(0);


//turn off toolbar
//----------------
id=curFig.figure_id;
tb=toolbar(id); //preserve setting
toolbar(id,"off");

// set double buffer mode to avoid blinking animation
//---------------------------------------------------
pix = curFig.pixmap; //preserve old setting
curFig.pixmap = "on";

// Creates the 3D curve points
//----------------------------

np=10;
t=0:0.1:np*%pi;
	
I=1:0.5:30;
x = (t/np).*sin(t);
y = (t/np).*cos(t);
z = I(1)*t/(np*%pi); //initial z coordinates
	
//Creates and set graphical entities which represent the surface
//--------------------------------------------------------------	
param3d(x,y,z,35,45);
p = gce(); //get the handle on 3D polyline
title("3d curve","fontsize",3);
curAxe = gca(); //handle on the currents axes
curAxe.tight_limits = "on";


//Use real time to slow down the animation loop
//---------------------------------------------
realtimeinit(0.03); //set time step (0.03 seconds)  and date reference
for i=2:(max(size(I)))
  realtime(i) //wait till date 0.1*i seconds
  drawlater(); //disable automatic redrawing
  p.data(:,3)=(I(i)*t/(np*%pi))'; //change the Z coordinates
  curAxe.data_bounds(2,3)=I(i);  //change the max Z axes value
  drawnow(); //enable automatic redraw
  show_pixmap();//send  buffer to screen
end


// Reset initial properties
//--------------------------------
toolbar(id,tb);
curFig.pixmap = pix;
