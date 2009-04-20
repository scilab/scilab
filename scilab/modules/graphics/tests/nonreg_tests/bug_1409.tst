// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 1342 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1342
//
// <-- Short Description -->
// Clipping does not work in 3D.
// 

lines(0);

M = 10;
N = 10;
//Define and Discretize Domain (ft)
xmax=50;           
xmin=-xmax;
ymax=50;            
ymin=-ymax;
dx=(xmax-xmin)/N;
dy=(ymax-ymin)/M;
x=xmin+(dx/2):dx:xmax-(dx/2);
y=ymin+(dy/2):dy:ymax-(dy/2);
[X,Y]=ndgrid(x,y);

Z=zeros(M,N)

//Plot Topographic Result in 3D
f0=scf(55);
a=get("current_axes");//get the handle of the newly created axes
a.data_bounds=[-10,10,-50,50,0,10]; //set the boundary values for the x, y and z coordinates.
xset("line style", 4) //Make Contour Lines Dashed
xset("fpf",string=" ")
plot3d(x,y,Z,alpha=45,theta=45,flag=[5,3,4],ebox=[-10,10,-50,50,0,10]);

// check that the surface is clipped within the axes box.

// same with this:
clf();
t=-%pi:0.3:%pi;plot3d(t,t,sin(t)'*cos(t),75,45,'X@Y@Z',[2,1,4],ebox=[-2,2,-2,2,-1,1]);
