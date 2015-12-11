//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

function demo_anim1()

    curFig             = scf(100001);
    clf(curFig,"reset");
    demo_viewCode("anim1.sce");

    show_window(); //raise the graphic window


    // set a new colormap
    //-------------------
    cmap= curFig.color_map; //preserve old setting
    curFig.color_map = hotcolormap(64);

    //The surface definition
    //----------------------
    x=%pi*(-5:5)/5;
    y=x;
    Z=sin(x)'*cos(y);

    //Creates and set graphical entities which represent the surface
    //--------------------------------------------------------------

    plot3d1(x,y,Z,35,45," ");
    s=gce(); //the handle on the surface
    s.color_flag=1; //assign facet color according to Z value
    title("rotation of a 3d surface","fontsize",3);
    curAxe = gca(); //handle on the currents axes

    //Set the evolution of the view angles Theta and Alpha
    //---------------------------------------------------
    st=2;
    T=35:st:80; //Theta
    A=45:st:80;  //Alpha
    Angles=[T A(1)*ones(A);T(1)*ones(T) A];

    //animation loop
    //--------------
    //use realtime to slow down the loop
    realtimeinit(0.1);//set time step and date reference
    for i=1:size(Angles,2) // loop on theta angle
        realtime(i); //wait till date 0.1*i seconds
        if ~is_handle_valid(curAxe) then
            break
        end
        curAxe.rotation_angles = Angles(:,i); //change the view angles property
    end

endfunction

demo_anim1();
clear demo_anim1;
