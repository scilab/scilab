//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Demonstrate animation based on the dynamic of a chain
// =============================================================================

function demo_anim7()

    // compute color of a set of facets, relative to the distance to the origin
    function [colors] = computeFacetsColor(xx, yy, zz, colormapSize)
        nbFacets = size(xx);
        colors = zeros(nbFacets(1), nbFacets(2));
        for i=1:nbFacets(1)
            for j=1:nbFacets(2)
                colors(i,j) = norm([xx(i,j), yy(i,j), zz(i,j)]) * colormapSize / 15;
            end
        end

    endfunction

    curFig             = scf(100001);
    clf(curFig,"reset");
    demo_viewCode("anim7.sce");

    show_window(); //raise the graphic window


    //Create the data
    //---------------
    colormapSize = 128;
    n=60;
    U = linspace(0,3*%pi,n);
    v = linspace(0,2*%pi,20);

    u = U(1:20);
    X = (cos(u).*u)'*(1+cos(v)/2);
    Y = (u/2)'*sin(v);
    Z = (sin(u).*u)'*(1+cos(v)/2);

    curFig.color_map = coppercolormap(colormapSize);

    [xx,yy,zz]=nf3d(X,Y,Z);//build facets


    // generate colors
    // colors depending on z, between 1 and colormap size
    facetsColors = computeFacetsColor(xx, yy, zz, colormapSize);


    //Creates and set graphical entities which represent the surface
    //--------------------------------------------------------------

    plot3d(xx,yy,list(zz,facetsColors)) //creates a Fac3d entity
    e = gce();
    data=e.data;
    title("shell","fontsize",3)
    isoview()

    curAxe=gca();
    // set 3D boundaries
    curAxe.data_bounds=[-15 -5 -10; 10  5  12];
    //set view angles
    curAxe.rotation_angles=[103 138];


    //animation loop
    //--------------
    //use realtime to slow down the loop
    K=20:4:n;
    realtimeinit(0.1);//set time step (0.1 seconds)  and date reference
    for k=2:size(K,"*")

        if ~is_handle_valid(curFig) then
            break;
        end

        realtime(k);

        //compute more facets
        u=U(K(k-1):K(k));
        X= (cos(u).*u)'*(1+cos(v)/2);
        Y= (u/2)'*sin(v);
        Z= (sin(u).*u)'*(1+cos(v)/2);
        [xx,yy,zz] = nf3d(X,Y,Z);//build facets

        //append new facets to the data data structure
        data.x=[data.x xx];
        data.y=[data.y yy];
        data.z=[data.z zz];
        data.color = [data.color, computeFacetsColor(xx, yy, zz, colormapSize)];

        if is_handle_valid(e) then
            e.data = data;// update the Fac3d entity
        end

    end

endfunction


demo_anim7();
clear demo_anim7;
