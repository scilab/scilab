// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) Samuel GOUGEON - 2013 : vectorization, code style
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function graypolarplot(theta,rho,z,varargin)
    [lhs,rhs] = argn(0)
    if rhs<=0 then
        rho = 1:0.2:4
        theta = (0:0.02:1)*2*%pi
        z = 30+round(theta'*(1+rho.^2))
        clf()
        f = gcf()
        f.color_map = hotcolormap(128)
        f.background= 128
        a = gca()
        a.background = 128
        a.foreground = 1
        graypolarplot(theta,rho,z)
        return
    end

    if rhs<3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "graypolarplot", 3));
    end


    R = max(rho)
    nv = size(varargin)
    if nv>=1
        strf = varargin(2)
    else
        strf = "030"
    end
    if nv>=2
        rect = varargin(4)
    else
        rect = [-R -R R R]*1.1
    end

    // drawlater
    fig = gcf();
    immediate_drawing = fig.immediate_drawing;
    fig.immediate_drawing = "off";

    axes = gca();
    axes.data_bounds = [rect(1), rect(2); rect(3), rect(4)];
    axes.clip_state = "clipgrf";

    drawGrayplot(theta,rho,z);
    isoview()

    axes.box = "off";
    axes.axes_visible = ["off","off","off"];
    axes.x_label.text = "";
    axes.y_label.text = "";
    axes.z_label.text = "";

    step = R/5
    r  = step;
    dr = 0.02*r;
    objectList = gce(); // get all the created objects to glue them at the end.
    for k = 1:4
        xarc(-r, r, 2*r, 2*r, 0, 360*64)
        objectList($ + 1) = gce();
        arc = gce();
        arc.line_style = 3;
        xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12), string(round(10*r)/10))
        objectList($ + 1) = gce();
        r=r+step
    end
    xarc(-r,r,2*r,2*r,0,360*64)
    objectList($ + 1) = gce();
    xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12), string(round(10*r)/10))
    objectList($ + 1) = gce();

    rect = xstringl(0,0,"360");
    w = rect(3);
    h = rect(4);
    r = R*1.05
    for k = 0:11
        xsegs([0 ; R*cos(k*(%pi/6))],[0 ; R*sin(k*(%pi/6))])
        objectList($ + 1) = gce();
        arc = gce();
        arc.line_style = 3;
        xstring((r+w/2)*cos(k*(%pi/6))-w/2, (r+h/2)*sin(k*(%pi/6))-h/2,string(k*30))
        objectList($ + 1) = gce();
    end

    // glue all the created objects
    glue(objectList);

    // drawnow
    fig.immediate_drawing = immediate_drawing;

endfunction
// ---------------------------------------------------------------------------

function [x,y] = polar2Cart(rho, theta)
    x = rho * cos(theta);
    y = rho * sin(theta);
endfunction
// ---------------------------------------------------------------------------

function [nbDecomp] = computeNeededDecompos(theta)
    // Compute the needed decomposition for each patch

    // minimal decompostion for each ring
    nbFactesPerRingMin = 512;

    nbDecomp = ceil(nbFactesPerRingMin / size(theta, "*"));

endfunction
// ---------------------------------------------------------------------------
function drawGrayplot(theta, rho, z)
    // draw only the colored part of the grayplot

    // the aim of the function is to draw a set of curved facets
    // In previous versions, it used arcs to perform this.
    // However, since arcs are drawn from the origin to the outside
    // there were overlapping and cause Z fighting in 3D.
    // Consequenlty we now decompose each curved facet into a set of rectangular
    // facets.

    nbRho = size(rho,"*");
    nbTheta = size(theta,"*");

    nbDecomposition = computeNeededDecompos(theta); // number of approximation facets

    // first step decompose theta in smaller intervals
    // Actually compute cosTheta and sinTheta for speed [vectorized]
    t = (1:nbDecomposition) / nbDecomposition
    [I,T] = meshgrid(theta, t)
    interpolatedData = T(:,2:$).*I(:,2:$) + (1-T(:,1:$-1)).*I(:,1:$-1)
    cosTheta = [cos(theta(1)) cos(interpolatedData(:))' ]
    sinTheta = [sin(theta(1)) sin(interpolatedData(:))' ]

    // compute the 4xnbFacets matrices for plot 3d
    //
    // get the 4 corners of a facet
    // (we minimize the memory footprint, since big transient and final matrices
    //  are built)
    Jmax = size(sinTheta,2)

    [R, C] = meshgrid(rho, cosTheta)
    R = R.*C
    clear C
    corner = R(1:Jmax-1,1:nbRho-1);    xCoords = corner(:)'
    corner = R(2:Jmax  ,1:nbRho-1);    xCoords(2,:) = corner(:)'
    corner = R(2:Jmax  ,2:nbRho);      xCoords(3,:) = corner(:)'
    corner = R(1:Jmax-1,2:nbRho);      xCoords(4,:) = corner(:)'

    [R, S] = meshgrid(rho, sinTheta)
    R = R.*S
    clear S
    corner = R(1:Jmax-1,1:nbRho-1);    yCoords = corner(:)'
    corner = R(2:Jmax  ,1:nbRho-1);    yCoords(2,:) = corner(:)'
    corner = R(2:Jmax  ,2:nbRho);      yCoords(3,:) = corner(:)'
    corner = R(1:Jmax-1,2:nbRho);      yCoords(4,:) = corner(:)'
    clear R

    // color is the same for each nbDecomposition facets
    // keep the 4 outside colors of the patch
    // to be able to switch between average or matlab color.
    i = 1:nbRho
    j = (0:Jmax-1)/ nbDecomposition + 1
    [I, J] = meshgrid(i,j)
    clear I
    corner = z(J(1:$-1,1)  , 1:$-1);    colors      = corner(:)'
    corner = z(J(1:$-1,1)+1, 1:$-1);    colors(2,:) = corner(:)'
    corner = z(J(1:$-1,1)+1, 2:$);      colors(3,:) = corner(:)'
    corner = z(J(1:$-1,1)  , 2:$);      colors(4,:) = corner(:)'
    clear J corner

    // flat plot
    nbQuadFacets = (nbRho - 1) * (Jmax - 1);
    zCoords = zeros(4, nbQuadFacets);

    // disable line draing and hidden color
    plot3d(xCoords, yCoords, list(zCoords,colors));
    gPlot = gce();
    gPlot.color_mode  = -1; // no wireframe
    gPlot.hiddencolor = 0; // no hidden color
    gPlot.color_flag  = 2; // average color on each facets

    // restore 2d view
    axes = gca();
    axes.view = "2d";

endfunction
