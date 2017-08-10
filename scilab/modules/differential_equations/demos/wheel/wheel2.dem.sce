// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) 2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_wheel2()

    thispath = get_absolute_file_path("wheel2.dem.sce");

    my_handle = scf(100001);
    clf(my_handle,"reset");

    exec(thispath+"wheel_show.sci");

    if ~(haveacompiler()) then
        messagebox(["Scilab doesn''t find a C compiler","This demo is disabled"],"modal");
        return;
    end

    mode(0);
    wheel_build_and_load();
    tmin   = 0.0;
    tmax   = 15;
    nn     = 300;
    times  = (0:(nn-1));
    times  = tmax*times/(nn-1) +tmin*((nn-1)*ones(times)-times);

    // Initial conditions
    // =========================================================================
    x0=[0;             // theta
    %pi/2+0.1;     // phi
    0;             // psi
    5.0;           // Dtheta
    0.0;           // Dphi
    4.0;           // Dpsi
    0;             // x
    0];            // y

    // Simulation
    // =========================================================================

    x=ode(x0,tmin,times,"wheel");

    clf(my_handle,"reset");
    wheel_show(x);

    ystr = [ "phi";"theta";"psi";"Dpsi";"Dtheta";"Dpsi";"x";"y"];
    flag = 2;

    if ~is_handle_valid(my_handle) then
        break;
    end

    while flag==2, [n1,n2]=size(x);
        if is_handle_valid(my_handle) then
            flag=x_choose(["Stop";"Go on"],"Choose");
            if flag==2 then
                x0 = evstr(x_mdialog(["Initial conditions"],ystr,string(x(:,n2))));
                x  = ode(x0,tmin,times,"wheel");
                clf(my_handle,"reset");
                if is_handle_valid(my_handle) then
                    wheel_show(x);
                end
            end
        end
    end

endfunction

demo_wheel2();
clear demo_wheel2;
