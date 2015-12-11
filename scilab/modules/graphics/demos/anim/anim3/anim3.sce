//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Demonstrate animation based on the rotation of Lorenz curve
// =============================================================================

function demo_anim3()

    mode(0)

    curFig             = scf(100001);
    clf(curFig,"reset");
    demo_viewCode("anim3.sce");

    show_window(); //raise the graphic window

    //turn off toolbar
    //----------------
    id=curFig.figure_id;

    //compute lorenz differential eqn solution
    //---------------------------------------------------

    y=ode(1.e-8*[1;1;1],0,0:0.005:50,"loren");
    [n1,n2]=size(y);
    wid = size(curFig.color_map,1); //number of colors

    //Creates and set graphical entities which represent the surface
    //--------------------------------------------------------------
    // we split the full trajectory in a set of polylines, each of them
    // having a different color
    drawlater(); //disable automatic redrawing
    for j=1:wid;//loop on the colors
        tt= ((j-1)*(n2/wid)+1):(j*(n2/wid));
        param3d(y(1,tt),y(2,tt),y(3,tt));
        e = gce(); //handle on the just created 3D polyline
        e.foreground = j;//set its color
    end
    title("Lorenz curve","fontsize",3);

    curAxe = gca();
    curAxe.box = "off";
    curAxe.axes_visible = "on";
    curAxe.grid= [12,12,12];
    drawnow()

    //Set the evolution of the view angle  Alpha
    //---------------------------------------------------
    A=35:2:160;

    //animation loop
    //--------------
    //use realtime to slow down the loop
    realtimeinit(0.1);//set time step (0.1 seconds)  and date reference
    for i=1:max(size(A))
        realtime(i); //wait till date 0.1*i seconds
        if is_handle_valid(curAxe) then
            curAxe.rotation_angles = [45,A(i)];
        else
            break;
        end
    end

endfunction

demo_anim3();
clear demo_anim3;
