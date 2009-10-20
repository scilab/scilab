// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function graypolarplot(theta,rho,z,varargin)
[lhs,rhs]=argn(0)
if rhs<=0 then
  rho=1:0.2:4;theta=(0:0.02:1)*2*%pi;
  z=30+round(theta'*(1+rho^2));
  clf();
  f=gcf();
  f.color_map=hotcolormap(128);
  f.background= 128;
  f.foreground=1;
  a=gca();
  a.background= 128;
  a.foreground=1;
  graypolarplot(theta,rho,z)
  return
end

if rhs<3 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "graypolarplot", 3));
end


R=max(rho)
nv=size(varargin)
if nv>=1 then strf=varargin(2),else  strf='030',end
if nv>=2 then rect=varargin(4),else  rect=[-R -R R R]*1.1,end

// drawlater
fig = gcf();
immediate_drawing = fig.immediate_drawing;
fig.immediate_drawing = "off";

axes = gca();
axes.data_bounds = [rect(1), rect(2); rect(3), rect(4)];
axes.clip_state = "clipgrf";

drawGrayplot(theta,rho,z);

objectList = gce(); // get all the created objects to glue them at the end.

axes.isoview = "on";
axes.box = "off";
axes.axes_visible = ["off","off","off"];
axes.x_label.text = "";
axes.y_label.text = "";
axes.z_label.text = "";

step=R/5
r=step;dr=0.02*r;
for k=1:4
  xarc(-r,r,2*r,2*r,0,360*64)
  objectList($ + 1) = gce();
  arc = gce();
  arc.line_style = 3;
  xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))
  objectList($ + 1) = gce();
  r=r+step
end
xarc(-r,r,2*r,2*r,0,360*64)
objectList($ + 1) = gce();
xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))
objectList($ + 1) = gce();

rect=xstringl(0,0,'360');w=rect(3);h=rect(4);d=sqrt(w^2+h^2)/1.8
r=R+d
for k=0:11
  xsegs([0;R*cos(k*(%pi/6))],[0;R*sin(k*(%pi/6))])
  objectList($ + 1) = gce();
  arc = gce();
  arc.line_style = 3;
  xstring(r*cos(k*(%pi/6))-w/2,r*sin(k*(%pi/6))-h/2,string(k*30))
  objectList($ + 1) = gce();
end

// glue all the created objects
glue(objectList);

// drawnow
fig.immediate_drawing = immediate_drawing;

endfunction

function [x,y] = polar2Cart(rho, theta)
  x = rho * cos(theta);
  y = rho * sin(theta);
endfunction

function [nbDecomp] = computeNeededDecompos(theta)
  // Compute the needed decomposition for each patch
  
  // minimal decompostion for each ring
  nbFactesPerRingMin = 512;
  
  nbDecomp = ceil(nbFactesPerRingMin / size(theta, '*'));
  
endfunction


function drawGrayplot(theta, rh, z)
// draw only the colored part of the grayplot

// the aim of the function is to draw a set of curved facets
// In previous versions, it used arcs to perform this.
// However, since arcs are drawn from the origin to the outside
// there were overlapping and cause Z fighting in 3D.
// Consequenlty we now decompose each curved facet into a set of rectangular
// facets.

nbRho = size(rho,'*');
nbTheta = size(theta,'*');

nbDecomposition = computeNeededDecompos(theta); // number of approximation facets

nbRefinedTheta = (nbTheta - 1) * nbDecomposition + 1;

// first step decompose theta in smaller intervals
// Actually compute cosTheta and sinTheta for speed
cosTheta = zeros(1, nbRefinedTheta);
sinTheta = zeros(1, nbRefinedTheta);

// first values
cosTheta(1) = cos(theta(1));
sinTheta(1) = sin(theta(1));

index = 2;
for i=1:(nbTheta - 1)
  for j=1:nbDecomposition
    t = j / nbDecomposition;
    interpolatedTheta = t * theta(i + 1) + (1 - t) * theta(i);
    cosTheta(index) = cos(interpolatedTheta);
    sinTheta(index) = sin(interpolatedTheta);
    index = index + 1;
  end
end


nbQuadFacets = (nbRho - 1) * (nbRefinedTheta - 1);

// allocate matrices
xCoords = zeros(4, nbQuadFacets);
yCoords = zeros(4, nbQuadFacets);
colors = zeros(4, nbQuadFacets);

index = 1;

// compute the 4xnbFacets matrices for plot 3d
for i=1:(nbRho - 1)
  for j=1:(nbRefinedTheta - 1);
    // get the 4 corners of a facet
    xCoords(:,index) = [rho(i) * cosTheta(j)
                        rho(i) * cosTheta(j + 1)
                        rho(i + 1) * cosTheta(j + 1)
                        rho(i + 1) * cosTheta(j)];

    yCoords(:,index) = [rho(i) * sinTheta(j)
                        rho(i) * sinTheta(j + 1)
                        rho(i + 1) * sinTheta(j + 1)
                        rho(i + 1) * sinTheta(j)];

    // color is the same for each nbDecomposition facets
	// retrieve the not refined index
	thetaIndex = (j - 1) / nbDecomposition + 1;
	// keep the 4 outside colors of the patch
	// to be able to switch between average or matlab color.
    colors(:,index) = [z(thetaIndex, i)
	                   z(thetaIndex + 1, i)
					   z(thetaIndex + 1, i + 1)
					   z(thetaIndex, i + 1)];

    index = index + 1;
  end
end


// flat plot
zCoords = zeros(4, nbQuadFacets);

// disable line draing and hidden color
plot3d(xCoords,yCoords,list(zCoords,colors));
gPlot = gce();
gPlot.color_mode = -1; // no wireframe
gPlot.hiddencolor = 0; // no hidden color
gPlot.color_flag = 2; // average color on each facets

// restore 2d view
axes = gca();
axes.view = "2d";


endfunction
