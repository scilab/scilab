//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Demonstrate animation based on the rotation of a 3D contour plot
// =============================================================================

function demo_anim4()

    mode(0);

    curFig             = scf(100001);
    clf(curFig,"reset");
    demo_viewCode("anim4.sce");

    show_window(); //raise the graphic window

    //The surface definition
    //----------------------
    x=linspace(-%pi,%pi,50);
    y=x;
    Z=sin(x)'*cos(y);

    //Creates and set graphical entities which represent the surface
    //--------------------------------------------------------------

    contour(x,y,Z,10,35,45,"X@Y@Z",[0,2,4])
    title("rotation of a 3d contour","fontsize",3)
    curAxe = gca();
    //Set the evolution of the view angle  Alpha
    //---------------------------------------------------
    A=35:80;
    //animation loop
    //--------------
    //use realtime to slow down the loop
    realtimeinit(0.05);//set time step (0.05 seconds)  and date reference
    for i=1:size(A,"*")
        realtime(i); //wait till date 0.05*i seconds
        if is_handle_valid(curAxe) then
            curAxe.rotation_angles = [45,A(i)];
        else
            break;
        end
    end

endfunction

demo_anim4()
clear demo_anim4;
