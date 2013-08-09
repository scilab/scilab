//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

function []=velo4p()
    // version with full wheels with  xgeom & xfpoly
    // ( geom performs the geometric calculations)
    function h=poly3d(x,y,z)
        xpoly(x,y);h=gce();h.data(:,3)=z
    endfunction
    t=t*180/%pi;
    p=p*180/%pi;

    drawlater()
    curAxe                 = gca();
    curAxe.axes_visible    = "off"
    curaxe.tight_limits    = "on";
    curaxe.isoview         = "on";
    curAxe.data_bounds     = [xmin ymin zmin;xmax ymax zmax];
    curAxe.rotation_angles = [t p];
    curAxe.title.text      = _("bike simulation, unstable trajectory");
    curAxe.title.font_size = 3;

    // The floor
    xfpoly([xmin xmax xmax xmin xmin],[ymin ymin ymax ymax ymin])
    e=gce();
    e.background    = color("lightgray");


    // Add decorations for fun
    poteau(xmin,(ymin+ymax)/2,0.4,0.4)
    arbre(xmax,ymax,0.1,0.1)
    arbre((xmax+xmin)/2,(ymin+ymax)/2,0.1,0.1)


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
    efront = poly3d(xpfront(1,1),xpfront(2,1),xpfront(3,1))
    drawnow()

    // animation
    if ~isdef("velo_rti") then   velo_rti=0.03; end
    realtimeinit(velo_rti);
    realtime(1)

    for i=1:1:n2

        if ~is_handle_valid(curAxe) then
            break;
        end

        realtime(i);

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

        if is_handle_valid(efront) then
            efront.data = [efront.data;
            xpfront(1,i),xpfront(2,i),xpfront(3,i)];
        end

        drawnow();
    end

endfunction

function []=poteau(xcenter,ycenter,xep,yep)
    // rajoute un poteau de centre xcenter,ycenter)
    // d'epaisseur xep et yep
    //!
    // Comment rajouter un petit cube
    xc=[xcenter-xep*1.001,xcenter-xep*0.999,xcenter+xep*0.999,xcenter+xep*1.001];
    yc=[ycenter-yep*1.001,ycenter-yep*0.999,ycenter+yep*0.999,ycenter+yep*1.001];
    zmat=zmin*ones(4,4);
    zmat(2:3,2:3)=zmax*ones(2,2);
    // grise du poteau
    potg=10
    plot3d(xc,yc,zmat,t,p," ",[potg,0,0]);
endfunction


function []=arbre(xcenter,ycenter,xep,yep)
    // rajoute un poteau de centre xcenter,ycenter)
    // d'epaisseur xep et yep
    //!
    // Comment rajouter un petit cube
    xc=[xcenter-xep*1.001,xcenter-xep*0.999,xcenter+xep*0.999,xcenter+xep*1.001];
    yc=[ycenter-yep*1.001,ycenter-yep*0.999,ycenter+yep*0.999,ycenter+yep*1.001];
    zmat=zmin*ones(4,4);
    zmat(2:3,2:3)=zmax*ones(2,2);
    // grise du poteau
    potg=10
    plot3d(xc,yc,zmat,t,p," ",[potg,0,0]);
    rand("uniform");
    teta=%pi*rand(1,100);
    phi=2*%pi*rand(1,100);
    r=[zmax-zmin];
    x=[xcenter*ones(1,100);xcenter*ones(1,100)+r*cos(teta).*cos(phi)];
    y=[ycenter*ones(1,100);ycenter*ones(1,100)+r*cos(teta).*sin(phi)];
    z=[zmax*ones(1,100);zmax*ones(1,100)+r*sin(teta)];
    param3d(x,y,z,t,p," ",[1,0],rect)
    curAxe = gca();
    curAxe.box = "off";
endfunction


