//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Demonstrate animation based on the evolution of a 3d curve
// =============================================================================

function demo_anim2()

    curFig             = scf(100001);
    clf(curFig,"reset");
    demo_viewCode("anim2.sce");

    show_window(); //raise the graphic window
    mode(0);

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

    drawlater();

    param3d(x,y,z,35,45);

    p            = gce(); //get the handle on 3D polyline
    p.foreground = color(255,0,0);
    p.thickness  = 2;

    curAxe              = gca(); //handle on the currents axes
    curAxe.tight_limits = "on";
    curAxe.background   = color(240,240,240);

    title("3d curve","fontsize",3);

    drawnow();

    //Use real time to slow down the animation loop
    //---------------------------------------------
    realtimeinit(0.03); //set time step (0.03 seconds)  and date reference
    for i=2:(max(size(I)))
        realtime(i); //wait till date 0.1*i seconds
        drawlater(); //disable automatic redrawing
        if is_handle_valid(p) &  is_handle_valid(curAxe) then
            p.data(:,3)=(I(i)*t/(np*%pi))'; //change the Z coordinates
            curAxe.data_bounds(2,3)=I(i);  //change the max Z axes value
            drawnow(); //enable automatic redraw
        else
            break;
        end
    end

endfunction

demo_anim2();
clear demo_anim2();
