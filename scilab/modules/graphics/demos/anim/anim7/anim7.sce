//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Demonstrate animation based on the dynamic of a chain
// =============================================================================

curFig             = scf(100001);
clf(curFig,"reset");

xselect(); //raise the graphic window

drawlater();


//Create the data
//---------------

n=60;
U = linspace(0,3*%pi,n);
v = linspace(0,2*%pi,20);

u = U(1:20);
X = (cos(u).*u)'*(1+cos(v)/2);
Y = (u/2)'*sin(v);
Z = (sin(u).*u)'*(1+cos(v)/2);

curFig.color_map = jetcolormap(128);

[xx,yy,zz]=nf3d(X,Y,Z);//build facets


// generate colors
// colors depending on z, between 1 and colormap size
colormapSize = 128;
facetsColors = (zz + 1) * (colormapSize - 1) / 2 + 1;


//Creates and set graphical entities which represent the surface
//--------------------------------------------------------------

plot3d(xx,yy,list(zz,facetsColors)) //creates a Fac3d entity
e = gce();
data=e.data;
title("shell","fontsize",3)

curAxe=gca();
// set 3D boundaries
curAxe.data_bounds=[-15 -5 -10; 10  5  12];
//set view angles
curAxe.rotation_angles=[103 138];

drawnow();


//animation loop
//--------------
//use realtime to slow down the loop
K=20:4:n;
realtimeinit(0.1);//set time step (0.1 seconds)  and date reference
for k=2:size(K,'*')
	
	realtime(k);
	
	drawlater();
	
	//compute more facets
	u=U(K(k-1):K(k));
	X= (cos(u).*u)'*(1+cos(v)/2);
	Y= (u/2)'*sin(v);
	Z= (sin(u).*u)'*(1+cos(v)/2);
	[xx,yy,zz]=nf3d(X,Y,Z);//build facets
	
	//append new facets to the data data structure
	data.x=[data.x xx];
	data.y=[data.y yy];
	data.z=[data.z zz];
	
	e.data=data;// update the Fac3d entity
	
	// == TEMPORARLILY !!! =============================================
	facetsColors = ([data.z zz] + 1) * (colormapSize - 1) / 9 + 1;
	plot3d([data.x xx],[data.y yy],list( [data.z zz],facetsColors) ) //creates a Fac3d entity
	// set 3D boundaries
	curAxe.data_bounds=[-15 -5 -10; 10  5  12];
	//set view angles
	curAxe.rotation_angles=[103 138];
	// == END OF TEMPORARLILY !!! ======================================
	
	
	drawnow();
	
end
