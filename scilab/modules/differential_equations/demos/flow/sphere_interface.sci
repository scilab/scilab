//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
// This file is distributed under the same license as the Scilab package.
//

function exesphere()
    // initialize the graphics and controls

    // initial point
    g_r = 0.7;//radius
    g_t = 250;//theta

    //initial speed
    g_V    = 1; //speed
    g_Vdir = 180; //speed orientation



    // Figure parameters
    // =========================================================================

    surface_frame_w = 600;     // surface frame width
    surface_frame_h = 600;     // surface frame height

    control_frame_w = 600;
    control_frame_h = 200;

    margin_x     = 15;      // Horizontal margin between each elements
    margin_y     = 15;      // Vertical margin between each elements

    axes_figure_w = 2*margin_x + max(surface_frame_w, control_frame_w);
    axes_figure_h = 3*margin_y + surface_frame_h + control_frame_h;

    // Figure creation
    // =========================================================================

    my_figure_handle = createMainWindow([axes_figure_w axes_figure_h], _("Sphere"), "main_figure");


    //Simulation parameters
    speed = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "speed", ...
        "m/s", ...
        [0 100], ...
        1, ...
        "speed");

    direction = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "direction", ...
        "deg", ...
        [0 100], ...
        50, ...
        "dir");

    r = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "r", ...
        "m", ...
        [0 99], ...
        0, ...
        "r");

    theta = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "theta", ...
        "deg", ...
        [0 100], ...
        1, ...
        "theta");

    //Create the controls in the window bottom
    // =========================================================================   
    param_list = list(r, theta, speed, direction);
    [plot_frame, param_frame, button_frame] = create_gui(my_figure_handle);
    create_param_zone(param_frame, param_list);
    create_buttons(button_frame);


    draw_sphere();
    draw_initial_point(g_r, g_t, g_V, g_Vdir, %T);



    //    sphere_create_gui()

    //Set control and graphics initial values
    // =========================================================================
    change_r(g_r)
    change_theta(g_t)
    change_speed(g_V)
    change_dir(g_Vdir)

    my_figure_handle.visible = "on";
endfunction

    // draw_sphere
    // =============================================================================

function draw_sphere()
    // draw half of a sphere (the flow lines surface )

    function [x, y, z]=sph(alp, tet)
        //parametric definition of a sphere
        x = r*cos(alp).*cos(tet)+orig(1)*ones(tet)
        y = r*cos(alp).*sin(tet)+orig(2)*ones(tet)
        z = r*sin(alp)+orig(3)*ones(tet);
    endfunction

    r    = 1;
    orig = [0 0 0];
    //create surface facets
    [xx, yy, zz]=eval3dp(sph, linspace(-%pi/2, 0, 40), linspace(0, %pi*2, 20));
    // draw facets
    plot3d(xx, yy, zz);

endfunction


    // draw_initial_point
    // =============================================================================
function draw_initial_point(g_r, g_t, g_V, g_Vdir, create)
    x = g_r*cos(g_t*%pi/180);
    y = g_r*sin(g_t*%pi/180);
    r = 0.05;
    z = -sqrt(1-x^2-y^2)+r;
    dx1 = 0.1*g_V*cos(g_Vdir*%pi/180);
    dy1 = 0.1*g_V*sin(g_Vdir*%pi/180);
    my_figure_handle = get("main_figure");
    my_figure_handle.immediate_drawing = "off";

    if create then
        //create the graphical object used to visualize the trajectory
        xpoly(0, 0)
        traj_handle                        =gce();
        traj_handle.thickness              = 2;
        traj_handle.foreground             = 5;
        //create the graphical object used to visualize the initial speed
        xpoly(0, 0)
        speed_handle                       =  gce();
        speed_handle.thickness             =  1;
        speed_handle.foreground            =  5;
        speed_handle.polyline_style        =  4;
        speed_handle.arrow_size_factor     =  2;
        //create the graphical objects used to visualize the initial position
        param3d([x-r, x+r], [y, y], [z, z]);
        e1                                 = gce();
        e1.thickness                       = 1;
        e1.foreground                      = 5;
        param3d([x, x], [y-r, y+r], [z, z]);
        e2                                 = gce();
        e2.thickness                       = 1;
        e2.foreground                      = 5;
        param3d([x, x], [y, y], [z-r, z+r]);
        e3                                 = gce();
        e3.thickness                       = 1;
        e3.foreground                      = 5;
        glue([traj_handle speed_handle e1 e2 e3])
    else
        curAxe = gca();
        initial=curAxe.children(1).children
        initial(1).data = [x, y, z-r;x, y, z+r];
        initial(2).data = [x, y-r, z;x, y+r, z];
        initial(3).data = [x-r, y, z;x+r, y, z];
    end
    z1=-sqrt(1-(x+dx1)^2-(y+dy1)^2)+r
    initial(4).data = [x, y, z;x+dx1, y+dy1, z1];

    my_figure_handle.immediate_drawing = "on";
endfunction

    // =============================================================================
    // The callbacks
    // =============================================================================

function change_r(r)
    //r slider callback
    // r is in [0 1]
    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");
    slider_r     = get("slider_r");

    value_r       = get("value_r");
    if argn(2)==1 then
        slider_r.Value=(r)*100;
    else
        r=slider_r.Value
        r=r/100
    end
    value_r.String=msprintf("%.3f", r)
    draw_initial_point(slider_r.Value/100, slider_theta.value*360/100, slider_speed.Value*20/100, slider_dir.value*360/100, %F);
endfunction


function change_theta(theta)
    //theta slider callback
    //theta is in [0 360]
    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");
    slider_r     = get("slider_r");

    value_theta   = get("value_theta");
    if argn(2)==1 then
        slider_theta.Value=(theta)*100/360;
    else
        theta=slider_theta.Value
        theta=theta*360/100
    end
    value_theta.String=msprintf("%.0f", theta)
    draw_initial_point(slider_r.Value/100, slider_theta.value*360/100, slider_speed.Value*20/100, slider_dir.value*360/100, %F);
endfunction

function change_speed(speed)
    //speed slider callback
    //speed is in [0 20]
    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");

    value_speed  = get("value_speed");
    if argn(2)==1 then
        slider_speed.Value=(speed)*100/20;
    else
        speed=slider_speed.Value
        speed=speed*20/100
    end
    value_speed.String=msprintf("%.0f", speed)
    draw_initial_point(slider_r.Value/100, slider_theta.value*360/100, slider_speed.Value*20/100, slider_dir.value*360/100, %F);
endfunction

function change_dir(dir)
    //direction slider callback
    //dir is in [0 360]
    slider_r      = get("slider_r");
    slider_theta  = get("slider_theta");
    slider_speed  = get("slider_speed");
    slider_dir    = get("slider_dir");
    value_dir     = get("value_dir");
    if argn(2)==1 then
        slider_dir.Value=(dir)*100/360;
    else
        dir=slider_dir.Value
        dir=dir*360/100
    end
    value_dir.String=msprintf("%.0f", dir)
    draw_initial_point(slider_r.Value/100, slider_theta.value*360/100, slider_speed.Value*20/100, slider_dir.value*360/100, %F);
endfunction

function start_simu()
    //start button callback
    my_figure_handle           = get("main_figure");
    fin                        = my_figure_handle.user_data
    my_figure_handle.user_data = %f

    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");
    t =            0:0.01:15
    Y = calculate_traj(slider_r.Value/100, slider_theta.value*360/100, ...
        slider_speed.Value*20/100, slider_dir.value*360/100, t)
    x = Y(1, :)
    y = Y(3, :)
    r = 0.1; //bias to have the curve above the surface
    z = -sqrt(1-x.^2-y.^2)+r;
    my_figure_handle = get("main_figure");
    curAxe = gca();
    traj_handle=curAxe.children(1).children(5);
    traj_handle.data=[x(1), y(1), z(1)];
    for k=2:size(x, "*")
        sleep(10)
        if execstr("fin=my_figure_handle.user_data", "errcatch")<>0|fin then break, end
        traj_handle.data=[traj_handle.data;[x(k), y(k), z(k)]];
    end
    my_figure_handle.user_data=%t
endfunction

function stop_simu()
    //stop button callback
    my_figure_handle = get("main_figure");
    fin = %T;
    my_figure_handle.user_data=fin
endfunction

function clear_simu()
    //clear button callback
    my_figure_handle = get("main_figure");
    fin=my_figure_handle.user_data
    if fin then
        my_figure_handle = get("main_figure");
        curAxe = gca();
        traj_handle=curAxe.children(1).children(5);
        traj_handle.data=[0, 0, 0];
    end
endfunction
