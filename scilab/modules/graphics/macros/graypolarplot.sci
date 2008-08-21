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
  xbasc();
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


R=max(rho)
nv=size(varargin)
if nv>=1 then strf=varargin(2),else  strf='030',end
if nv>=2 then rect=varargin(4),else  rect=[-R -R R R]*1.1,end

// drawlater
fig = gcf();
immediate_drawing = fig.immediate_drawing;
fig.immediate_drawing = "off";

//plot2d(0,0,1,strf,' ',rect)
axes = gca();
axes.data_bounds = [rect(1), rect(2); rect(3), rect(4)];
axes.clip_state = "clipgrf";
//[rho,k]=sort(rho);z=z(:,k);

//nt=size(theta,'*');theta=matrix(theta,1,-1)*180/%pi
drawGrayplot(theta,rho,z);

axes.isoview = "on";
axes.box = "off";
axes.axes_visible = ["off","off","off"];
axes.x_label.text = "";
axes.y_label.text = "";
axes.z_label.text = "";

// bug here, we need to draw the axes in order to get correct bounding box.
draw(axes);


step=R/5
r=step;dr=0.02*r;
for k=1:4
  xarc(-r,r,2*r,2*r,0,360*64)
  arc = gce();
  arc.line_style = 3;
  xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))
  r=r+step
end
xarc(-r,r,2*r,2*r,0,360*64)
xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))

rect=xstringl(0,0,'360');w=rect(3);h=rect(4);d=sqrt(w^2+h^2)/1.8
r=R+d
for k=0:11
  xsegs([0;R*cos(k*(%pi/6))],[0;R*sin(k*(%pi/6))])
  arc = gce();
  arc.line_style = 3;
  xstring(r*cos(k*(%pi/6))-w/2,r*sin(k*(%pi/6))-h/2,string(k*30))
end

// drawnow
fig.immediate_drawing = immediate_drawing;

endfunction

function [x,y] = polar2Cart(rho, theta)
  x = rho * cos(theta);
  y = rho * sin(theta);
endfunction


function [x,y] = decomposeCurvedFacet(rho0, rho1, theta0, theta1)
  // approximate a curved facet into a set of rectangular ones
  // rho0, rho1, theta0, theta1 defined the boundaries of the facet
  // x and y are 4xnbDecompostion matrices. Each row of these matrices
  // contains the X or Y coordinates of the 4 vertices of an approximating
  // quad.
  // The decomposition is performed along the theta angle
 
  nbDecomposition = 32; // number of approximation facets

  [x0, y0] = polar2Cart(rho0, theta0);
  x(1,1) = x0;
  y(1,1) = y0;
  [x1, y1] = polar2Cart(rho1, theta0);
  x(2,1) = x1;
  y(2,1) = y1;
  for i=1:(nbDecomposition - 1)
    t = i / nbDecomposition
    // get an interpolated theta between theta0 and theta1
    thetaInterp = t * theta1 + (1 - t) * theta0;
    [x0, y0] = polar2Cart(rho1, thetaInterp);
    x(3,i) = x0;
    y(3,i) = y0;
    [x1, y1] = polar2Cart(rho0, thetaInterp);
    x(4,i) = x1;
    y(4,i) = y1;

    // the nexr quad first two points are the previous one last two
    x(1,i+1) = x(4,i);
    y(1,i+1) = y(4,i);
    x(2,i+1) = x(3,i);
    y(2,i+1) = y(3,i);
  end

  [x0, y0] = polar2Cart(rho1, theta1);
  x(3,nbDecomposition) = x0;
  y(3,nbDecomposition) = y0;
  [x1, y1] = polar2Cart(rho0, theta1);
  x(4,nbDecomposition) = x1;
  y(4,nbDecomposition) = y1;

endfunction


function drawGrayplot(theta, rh, z)
// draw only the colored part of the grayplot

// the aim of the function is to draw a set of curved facets
// In previous versions, it used arcs to perform this.
// However, since arcs are drawn from the origin to the outside
// there were overlapping and cause Z fighting in 3D.
// Consequnelty we now decompose each curved facet into a set of rectangular
// facets.

nbRho = size(rho,'*');
nbTheta = size(theta,'*');

xCoords = [];
yCoords = [];
colors = [];

// compute the 4xnbFacets matrices for plot 3d
for i=1:(nbRho - 1)
  for j =1:(nbTheta - 1);
    // compute the decomposition of a curved facet
    [xFacet, yFacet] = decomposeCurvedFacet(rho(i), rho(i + 1), theta(j), theta(j + 1));
    xCoords = [xCoords, xFacet];
    yCoords = [yCoords, yFacet];
    colors = [colors, z(j,i) * ones(1,size(xFacet,2))];
  end
end

// flat plot
zCoords = zeros(size(xCoords,1),size(xCoords,2));



plot3d(xCoords,yCoords,list(zCoords,colors));
gPlot = gce();
gPlot.color_mode = -1;
gPlot.hiddencolor = 0;

axes = gca();
axes.view = "2d";


endfunction
