//
// Scilab ( http://www.scilab.org/) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// Cylinder interface functions
// =============================================================================

// =============================================================================
// Step 1, set up the gradient
// =============================================================================
function execylinder()
    g_gravity = 9.81;
    g_pente = 14;

    g_x = 0.6;
    g_y = 0.7;

    g_V = 1;
    g_Vdir = 180;

    g_alpha = 35;
    g_theta = 45;

    // Figure parameters
    // =========================================================================

    surface_frame_w = 600; // surface frame width
    surface_frame_h = 600; // surface frame height

    control_frame_w = 600;
    control_frame_h = 200;

    margin_x = 15; // Horizontal margin between each elements
    margin_y = 15; // Vertical margin between each elements

    axes_figure_w = 2*margin_x + max(surface_frame_w, control_frame_w);
    axes_figure_h = 3*margin_y + surface_frame_h + control_frame_h;

    // Figure creation
    // =========================================================================

    my_figure_handle = createMainWindow([axes_figure_w axes_figure_h], _("Cylinder"), "main_figure");
    my_figure_handle.immediate_drawing = "off";

    //Simulation parameters
    gravity = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "gravity", ...
        "m/s²", ...
        [0.1 100], ...
        9.81, ...
        "g");

    x = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "x", ...
        "m", ...
        [0 100], ...
        0.6, ...
        "x");

    speed = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "speed", ...
        "m/s", ...
        [0 100], ...
        0, ...
        "speed");

    slope = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "slope", ...
        "deg", ...
        [0 100], ...
        14, ...
        "slope");

    y = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "y", ...
        "m", ...
        [1 100], ...
        0.7, ...
        "y");

    direction = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
        "direction", ...
        "deg", ...
        [0 100], ...
        180, ...
        "direction");

    //Setting up the GUI
    param_list = list(gravity, x, speed, slope, y, direction);
    [plot_frame, param_frame, button_frame] = create_gui(my_figure_handle);
    create_param_zone(param_frame, param_list);
    create_buttons(button_frame);

    draw_cylinder(g_pente, %T);

    change_x(g_x)
    change_y(g_y)
    change_speed(g_V)
    change_direction(g_Vdir)
    change_g(g_gravity)

    draw_initial_point(g_x, g_y, g_V, g_Vdir, g_pente, %T)

    my_figure_handle.immediate_drawing = "on";

    my_figure_handle.visible = "on";
endfunction

// draw_cylinder
// =============================================================================
function draw_cylinder(g_pente, orig)
    a = -tan(g_pente/180*%pi);
    xx = -1:0.1:1;
    yy = (0:1:14)';

    if orig then
        Z = -sqrt(1-ones(yy)*(xx.^2))+a*yy*ones(xx);
        plot3d(xx, yy, Z');
    else
        my_figure_handle = get("main_figure");
        my_axe = gca()
        Z = -sqrt(1-ones(yy)*(xx.^2))+a*yy*ones(xx);
        e = my_axe.children(size(my_axe.children, "*"));
        e.data.z = Z';
        my_axe.data_bounds(1, 3) = min(Z);
    end
endfunction

    // draw_initial_point
    // =============================================================================
function draw_initial_point(x, y, g_V, g_Vdir, slope, create)
    a = -tan(slope*%pi/180);
    r = 0.1;
    z = -sqrt(1-x^2)+a*y+r;
    dx1 = g_V*cos(g_Vdir*%pi/180);
    dy1 = g_V*sin(g_Vdir*%pi/180);
    my_figure_handle = get("main_figure");
    my_figure_handle.immediate_drawing = "off";

    if create then
        //create the graphical object used to visualize the trajectory
        xpoly(0, 0)
        traj_handle =gce();
        traj_handle.thickness = 2;
        traj_handle.foreground = 5;
        //create the graphical object used to visualize the initial speed
        xpoly(0, 0)
        speed_handle = gce();
        speed_handle.thickness = 1;
        speed_handle.foreground = 5;
        speed_handle.polyline_style = 4;
        speed_handle.arrow_size_factor = 2;
        //create the graphical objects used to visualize the initial position
        param3d([x-r, x+r], [y, y], [z, z]);
        e1 = gce();
        e1.thickness = 1;
        e1.foreground = 5;
        param3d([x, x], [y-r, y+r], [z, z]);
        e2 = gce();
        e2.thickness = 1;
        e2.foreground = 5;
        param3d([x, x], [y, y], [z-r, z+r]);
        e3 = gce();
        e3.thickness = 1;
        e3.foreground = 5;
        glue([traj_handle speed_handle e1 e2 e3])
    else
        curAxe = gca();
        initial=curAxe.children(1).children
        initial(1).data = [x, y, z-r;x, y, z+r];
        initial(2).data = [x, y-r, z;x, y+r, z];
        initial(3).data = [x-r, y, z;x+r, y, z];
    end
    z1=-sqrt(1-(x+dx1)^2)+a*(y+dy1)+r;
    initial(4).data = [x, y, z;x+dx1, y+dy1, z1];

    my_figure_handle.immediate_drawing = "on";
endfunction


// =============================================================================
// The callbacks
// =============================================================================
function change_x(x)
    //x slider callback
    // x is in [-0.9 0.9]
    slider_x = get( "slider_x");
    slider_y = get( "slider_y");
    slider_slope = get( "slider_slope");
    slider_speed = get( "slider_speed");
    slider_direction = get( "slider_direction");

    value_x = get( "value_x");
    if argn(2)==1 then
        slider_x.Value=(x+0.9)*100/1.8;
    else
        x=slider_x.Value
        x=-0.9+x*1.8/100
    end
        value_x.String=msprintf("%.3f", x)
        draw_initial_point(-0.9+slider_x.Value*1.8/100, slider_y.value*3/100, ..
        slider_speed.Value*2/100, slider_direction.value*360/100, slider_slope.Value*70/100, %F);
endfunction

function change_y(y)
    //y slider callback
    //y is in [0 3]
    slider_x = get( "slider_x");
    slider_y = get( "slider_y");
    slider_slope = get( "slider_slope");
    slider_speed = get( "slider_speed");
    slider_direction = get( "slider_direction");

    value_y = get( "value_y");
    if argn(2)==1 then
        slider_y.Value=(y)*100/3;
    else
        y=slider_y.Value
        y=y*3/100
    end
    value_y.String=msprintf("%.3f", y)
    draw_initial_point(-0.9+slider_x.Value*1.8/100, slider_y.value*3/100, ..
    slider_speed.Value*2/100, slider_direction.value*360/100, slider_slope.Value*70/100, %F);
endfunction

function change_speed(speed)
    //speed slider callback
    //speed is in [0 2]
    slider_x = get( "slider_x");
    slider_y = get( "slider_y");
    slider_slope = get( "slider_slope");
    slider_speed = get( "slider_speed");
    slider_direction = get( "slider_direction");

    value_speed = get( "value_speed");
    if argn(2)==1 then
        slider_speed.Value=(speed)*100/2;
    else
        speed=slider_speed.Value
        speed=speed*2/100
    end
    value_speed.String=msprintf("%.2f", speed)
    draw_initial_point(-0.9+slider_x.Value*1.8/100, slider_y.value*3/100, ..
    slider_speed.Value*2/100, slider_direction.value*360/100, slider_slope.Value*70/100, %F);
endfunction

function change_slope(slope)
    //slope slider callback
    //slope is in [0 70]
    slider_x = get( "slider_x");
    slider_y = get( "slider_y");
    slider_slope = get( "slider_slope");
    slider_speed = get( "slider_speed");
    slider_direction = get( "slider_direction");

    value_slope = get( "value_slope");
    if argn(2)==1 then
        slider_slope.Value=(slope)*100/70;
    else
        slope=slider_slope.Value
        slope=slope*70/100
    end
    value_slope.String=msprintf("%.2f", slope)
    g_pente=slope
    draw_cylinder(g_pente, %f);
    draw_initial_point(-0.9+slider_x.Value*1.8/100, slider_y.value*3/100, ..
    slider_speed.Value*2/100, slider_direction.value*360/100, slider_slope.Value*70/100, %F);
endfunction

function change_direction(direction)
    //direction slider callback
    //dir is in [0 360]
    slider_x = get( "slider_x");
    slider_y = get( "slider_y");
    slider_slope = get( "slider_slope");
    slider_speed = get( "slider_speed");
    slider_direction = get( "slider_direction");

    value_direction = get( "value_direction");
    if argn(2)==1 then
        slider_direction.Value=(direction)*100/360;
    else
        direction=slider_direction.Value
        direction=direction*360/100
    end
    value_direction.String=msprintf("%.0f", direction)
    draw_initial_point(-0.9+slider_x.Value*1.8/100, slider_y.value*3/100, ..
    slider_speed.Value*2/100, slider_direction.value*360/100, slider_slope.Value*70/100, %F);
endfunction

function change_g(gravity)
    //gravity slider callback
    //gravity is in [0 10]
    slider_x = get( "slider_x");
    slider_y = get( "slider_y");
    slider_slope = get( "slider_slope");
    slider_speed = get( "slider_speed");
    slider_direction = get( "slider_direction");
    slider_g = get( "slider_g");

    value_g = get( "value_g");
    if argn(2)==1 then
        slider_g.Value=(gravity)*100/10;
    else
        gravity=slider_g.Value
        gravity=gravity*10/100
    end
    value_g.String=msprintf("%.2f", gravity)
endfunction


function start_simu()
    //start button callback
    my_figure_handle = get("main_figure");
    fin = my_figure_handle.user_data
    my_figure_handle.user_data = %f

    slider_x = get( "slider_x");
    slider_y = get( "slider_y");
    slider_slope = get( "slider_slope");
    slider_speed = get( "slider_speed");
    slider_direction = get( "slider_direction");
    slider_g = get( "slider_g");


    t = 0:0.01:10; //1001 track points

    x = -0.9+slider_x.Value*1.8/100
    y = slider_y.value*3/100
    V = slider_speed.Value*2/100
    Vdir = slider_direction.value*360/100
    slope = slider_slope.Value*70/100
    gravity = slider_g.Value*10/100
    Y = calculate_traj(x, y, V, Vdir, t, gravity, slope)
    x = Y(1, :)
    y = Y(3, :)
    r = 0.1; //bias to have the curve above the surface
    a = -tan(slope/180*%pi);
    z = -sqrt(1-x.^2)+a*y+r;
    my_figure_handle = get("main_figure");
    curAxe = gca();
    traj_handle=curAxe.children(1).children(5);
    traj_handle.data=[x(1), y(1), z(1)];
    for k=2:size(x, "*")
        sleep(3)
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
