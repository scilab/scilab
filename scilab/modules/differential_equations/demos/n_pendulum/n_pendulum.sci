//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
//
// This file is distributed under the same license as the Scilab package.
//

function demo_pendulum()

    function []=npend_build_and_load()

        // since this demo can be run by someone
        // who has no write access in this directory
        // we use TMPDIR

        if ~c_link("npend") then
            path   = SCI+"/modules/differential_equations/demos/n_pendulum/Maple/"
            cdpath = pwd();
            chdir(TMPDIR);
            fcode = mgetl(path+"dlslv.f");mputl(fcode,"dlslv.f");
            fcode = mgetl(path+"np.f");mputl(fcode,"np.f");
            fcode = mgetl(path+"npend.f");mputl(fcode,"npend.f");
            fcode = mgetl(path+"sci_np.c");mputl(fcode, "sci_np.c");
            files = ["npend.f","np.f","dlslv.f"];
            ilib_verbose(0);
            lib = ilib_for_link(["npend";"np"], ["dlslv.f", "npend.f", "np.f"], [],"f");
            link(lib, "npend", "f");
            ilib_build("gw",["np","sci_np"],"sci_np.c",basename(lib));
            exec("loader.sce",-1);
            chdir(cdpath);
        end
    endfunction

    function draw_chain_from_angles(a,r,job)
        // a the angles , a(i,j) is the angle of node i a time t(j)
        // r the segments half length
        if argn(2)<3 then job=0,end
        n2=size(a,2);
        // build the links positions
        x=[0*ones(1,n2);cumsum(2*diag(r)*cos(a),1)];
        y=[0*ones(1,n2);cumsum(2*diag(r)*sin(a),1)];
        draw_chain_from_coor(x,y,job)
    endfunction


    function draw_chain_from_coor(x,y,job)

        my_handle = scf(100001);
        clf(my_handle,"reset");
        my_handle.immediate_drawing = "off"; //not to see the intermediate graphic steps

        // x,y the coordinates ,
        //   x(i,j), y(i,j) is the coordinate of node i a time t(j)
        // r the segments half length
        if argn(2)<3 then job=0,end
        [n1,n2]=size(x);

        //set the frame
        a=gca()
        a.data_bounds=2*[-n1,-n1;n1,0];

        //create one polyline and one polymark with the initial position
        xsegs([x(1:$-1,1)';x(2:$,1)'],[y(1:$-1,1)';y(2:$,1)'],1:n1-1);
        p=gce();
        p.thickness=4;
        xpoly(x(:,1),y(:,1),"lines");
        p1=gce();

        p1.mark_style=3;
        p1.mark_size_unit="point";
        p1.mark_size=6;

        if job==1 then
            //bound trajectory
            xpoly(x($,1)*ones(2,1),y($,1)*ones(2,1),"lines");
            t=gce();t.line_style=2;
        end

        my_handle.immediate_drawing = "on";

        ind=[1;(2:n1-1)'.*.ones(2,1);n1]
        realtimeinit(0.1)
        for j=1:n2,

            if ~is_handle_valid(my_handle) then
                break;
            end

            realtime(j) //to slow down the drawing

            if is_handle_valid(my_handle) then
                my_handle.immediate_drawing = "off";
            end

            // update chain coordinates
            if is_handle_valid(p1) then
                p1.data = [x(:,j),y(:,j)];
            end

            if is_handle_valid(p) then
                p.data = [x(ind,j),y(ind,j)];
            end

            // add a trajectory point
            if job==1 & is_handle_valid(t) then
                t.data=[t.data;[x($,j),y($,j)]];
            end

            if is_handle_valid(my_handle) then
                my_handle.immediate_drawing = "on";
            end
        end
    endfunction


    if ~(haveacompiler()) then
        messagebox(["Scilab doesn''t find a C compiler","This demo is disabled"],"modal");
    else
        npend_build_and_load();
        n  = np();
        r  = ones(1,n);
        m  = ones(1,n);
        j  = ones(1,n);
        g  = 9.81;
        y0 = 0*ones(2*n,1);tt=0:0.05:10;
        yt = ode(y0,0,tt,"npend");
        draw_chain_from_angles(yt(1:$/2,:),r,1);
    end

endfunction
