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

function demo_anim6()

    //Read the precomputed data of the chain evolution
    path = get_absolute_file_path("anim6.sce");
    yt = read(path + "yt.wrt", 20, 201);  //load data

    curFig = scf(100001);
    clf(curFig, "reset");
    demo_viewCode("anim6.sce");

    drawnow();

    show_window(); //raise the graphic window

    //The initial chain position definition
    //-------------------------------------
    angles = yt(1:$/2,1);

    //Creates and set graphical entities which represent the surface
    //--------------------------------------------------------------

    r = ones(angles); //length of each link
    //coordinates of the links boundaries
    x = [0;cumsum(2*r.*cos(angles))];
    y = [0;cumsum(2*r.*sin(angles))];
    n1 = size(x, "*");

    //set the axes boundary
    curAxes = gca();
    curAxes.box = "on";
    curAxes.data_bounds = 2*sum(r)*[-1,-1;1,0.1];
    curAxes.title.text = "N link pendulum movement";
    curAxes.title.font_size = 3;

    colors = 1:n1;
    colors(8) = n1 + 1;
    xpolys([x(1:($-1)), x(2:$)]', [y(1:($-1)),y(2:$)]', colors);
    e = gce(); //compound with 10 polylines as children
    e.children(:).thickness = 4;
    drawlater();

    //animation loop
    //--------------
    //use realtime to slow down the loop
    realtimeinit(0.04);//set time step (0.04 seconds)  and date reference

    for j = 1:size(yt,2)

        if ~is_handle_valid(e) then
            return
        end

        realtime(j);//wait till date 0.04*i seconds
        drawlater();
        angles = yt(1:$/2, j);
        r = ones(angles); //length of each link

        x = [0;cumsum(2*r.*cos(angles))];
        y = [0;cumsum(2*r.*sin(angles))];
        n = size(r, "*");

        for i = 1:n
            if is_handle_valid(e) then
                hChild = e.children(n+1-i);
                if is_handle_valid(hChild) then
                    hChild.data = [x(i), y(i); x(i+1), y(i+1)];
                else
                    return
                end
            else
                return
            end
        end
        drawnow();
    end

endfunction


demo_anim6();
clear demo_anim6;
