//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

function []=velo1()
    // "full wheel" version
    function h=poly3d(x,y,z)
        xpoly(x,y);h=gce();h.data(:,3)=z
    endfunction

    xe=[xmin;xmax;xmax;xmin;xmin]
    ye=[ymin;ymin;ymax;ymax;ymin]
    ze=[zmin;zmin;zmin;zmin;zmin];
    drawlater()
    curAxe                 = gca();
    curAxe.margins         = [0 0 0.1 0];
    curAxe.axes_visible    = "off"
    curaxe.tight_limits    = "on";
    curaxe.isoview         = "on";
    curAxe.data_bounds     = [xmin ymin zmin;xmax ymax min(xmax-xmin, ymax-ymin)/2];
    curAxe.rotation_angles = [82,-3];
    curAxe.title.text      = _("bike simulation, stable trajectory")
    curAxe.title.font_size = 3;

    // The floor
    xfpoly([xmin xmax xmax xmin xmin],[ymin ymin ymax ymax ymin])
    e=gce();
    e.background    = color("lightgray");


    [n1,n2]=size(xfrontar);

    // Build the bicycle graphical objects
    e3=poly3d(xf(:,1),yf(:,1),zf(:,1))
    //rear wheel
    e2=poly3d(xrearar(:,1),yrearar(:,1),zrearar(:,1))
    // front wheel
    e1=poly3d(xfrontar(:,1),yfrontar(:,1),zfrontar(:,1))
    // rear trace
    erear  = poly3d(xprear(1,1),xprear(2,1),xprear(3,1))
    // front trace
    //  efront = poly3d(xpfront(1,1),xpfront(2,1),xpfront(3,1))
    drawnow()

    // animation
    if ~isdef("velo_rti") then   velo_rti=0.05;end
    realtimeinit(velo_rti);
    realtime(0)

    curAxe = gca();

    for i=2:1:n2

        realtime(i);

        if ~is_handle_valid(curAxe) then
            break;
        end

        drawlater();

        if is_handle_valid(e1) then
            e1.data     = [xfrontar(:,i) yfrontar(:,i) zfrontar(:,i)];
        end

        if is_handle_valid(e2) then
            e2.data     = [xrearar(:,i)  yrearar(:,i)  zrearar(:,i) ];
        end

        if is_handle_valid(e3) then
            e3.data     = [xf(:,i) yf(:,i) zf(:,i)];
        end

        if is_handle_valid(erear) then
            erear.data  = [erear.data;
            xprear(1,i),xprear(2,i),xprear(3,i)];
        end

        drawnow();

    end

endfunction


