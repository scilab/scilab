//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// BlackHole interface functions
// =============================================================================
// exeblackhole
// =============================================================================
function exeblackhole()
    //initial point
    g_r = 1.1;
    g_t = 41;

    //initial speed
    g_V    = 1.65;
    g_Vdir = 165;

    // Figure parameters
    surface_frame_w = 600;     // surface frame width
    surface_frame_h = 600;     // surface frame height

    control_frame_w = 600;
    control_frame_h = 200;

    margin_x = 15;      // Horizontal margin between each elements
    margin_y = 15;      // Vertical margin between each elements

    axes_figure_w = 2 * margin_x + max(surface_frame_w, control_frame_w);
    axes_figure_h = 3 * margin_y + surface_frame_h + control_frame_h;

    // Figure creation
    my_figure_handle = createMainWindow([axes_figure_w axes_figure_h], _("Blackhole"), "main_figure");
    my_figure_handle.immediate_drawing = "off";

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
    [4 100], ...
    1, ...
    "r");

    theta = tlist(["sim_param", "name", "unit", "range", "default", "tag"], ...
    "theta", ...
    "deg", ...
    [0 100], ...
    1, ...
    "theta");

    //Create the controls in the window bottom
    param_list = list(r, theta, speed, direction);
    [plot_frame, param_frame, button_frame] = create_gui(my_figure_handle);
    create_param_zone(param_frame, param_list);
    create_buttons(button_frame);


    draw_blackhole();
    draw_initial_point(g_r, g_t, g_V, g_Vdir, %T);


    //Set control and graphics initial values
    change_r(g_r)
    change_theta(g_t)
    change_speed(g_V)
    change_dir(g_Vdir)
    my_figure_handle.visible = "on";
endfunction

// draw_surface
// =============================================================================
function draw_blackhole()
    r    = 1;
    orig = [0 0 0];
    xx   = -1.85:0.1:1.85;
    yy   = (-1.85:0.1:1.85)';
    zt   = sqrt((ones(yy)*(xx .^ 2) + (yy .^ 2) * ones(xx)));
    zz   = -1 * ones(zt) ./ zt;

    my_handle = get("main_figure");

    my_handle.immediate_drawing = "off";
    mesh(xx, yy, zz);
    e = gce();
    e.color_mode = 0;
    e.foreground = color("blue");
    my_handle.immediate_drawing = "on";
endfunction

// draw_initial_point
// =============================================================================
function draw_initial_point(g_r, g_t, g_V, g_Vdir, create)
    x = g_r * cos(g_t * %pi / 180);
    y = g_r * sin(g_t * %pi / 180);
    r = 0.05;
    z = -1.0 ./ sqrt(x ^ 2 + y ^ 2) + r;
    dx1 = g_V * cos(g_Vdir * %pi / 180);
    dy1 = g_V * sin(g_Vdir * %pi / 180);
    my_figure_handle = get("main_figure");
    my_figure_handle.immediate_drawing = "off";

    if create then
        //create the graphical object used to visualize the trajectory
        xpoly(0, 0)
        traj_handle                        = gce();
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
        param3d([x - r x + r], [y y], [z z]);
        e1                                 = gce();
        e1.thickness                       = 1;
        e1.foreground                      = 5;
        param3d([x x], [y - r y + r], [z z]);
        e2                                 = gce();
        e2.thickness                       = 1;
        e2.foreground                      = 5;
        param3d([x x], [y y], [z - r z + r]);
        e3                                 = gce();
        e3.thickness                       = 1;
        e3.foreground                      = 5;
        glue([traj_handle speed_handle e1 e2 e3])
    else
        curAxe = gca();
        initial = curAxe.children(1).children
        initial(1).data = [x  y z - r ; x y z + r];
        initial(2).data = [x y - r z ; x y + r z];
        initial(3).data = [x - r y z ; x + r y z];
    end
    z1= -1.0 ./ sqrt((x + dx1) ^ 2 + (y + dy1) ^ 2) + r;
    initial(4).data = [x y z ; x + dx1 y + dy1 z1];

    my_figure_handle.immediate_drawing = "on";
endfunction

// =============================================================================
// The gui controls builder
// =============================================================================
function blackhole_create_gui()
    //initial values
    g_t     = 0;
    g_r     = 0;
    g_speed = 0;
    g_Vdir  = 0;

    my_figure_handle = get("main_figure");

    frame_x      = 25;
    frame_y      = 200;

    slider_height   = 15;
    slider_width    = 140;
    value_width     = 30;
    unit_width      = 30;
    text_width      = 50;
    y_margin        = 5;
    x_margin        = 10;
    title_width     =  text_width + slider_width + value_width + unit_width + 3 * x_margin

    x_text          = frame_x
    x_slider        = x_text + text_width + x_margin;
    x_value         = x_slider + slider_width + x_margin;
    x_unit          = x_value  + value_width + x_margin;

    y               = frame_y - 40;

    //Create the constraints for the gridbag simulation zone / position + speed frames
    gen_c = createConstraints("border", "bottom", [600 200]);

    //Create the layout options for the frame containing position and speed frames
    //1by2 grid with 20 pixel horizontal space for minimum padding
    posspeed_lay_opt = createLayoutOptions("grid", [1 2], [20 0]);


    gui_frame = uicontrol(my_figure_handle, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "border", ...
    "constraint", gen_c);

    //Frame containing position and speed
    gen_c.position = "center";
    frame_posspeed = uicontrol(gui_frame, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "grid", ...
    "layout_options", posspeed_lay_opt, ...
    "constraint", gen_c);

    //Borders and title around position or speed
    b_f_posspeed = createBorderFont("", 16);
    b_l_posspeed = createBorder("line", "navy", 1);
    b_posspeed = createBorder("titled", b_l_posspeed, "starting point", "center", "top", b_f_posspeed, "navy");

    //Create the internal layout options for the position and speed frames
    //2by1 grid with 10 pixel vertical space for minimum padding
    grid_lay_opt = createLayoutOptions("grid", [2 1], [0 10]);

    //Frame initspeed
    //title initial speed
    //vertical grid: speed_frame | 10 | direction_frame
    b_posspeed.title = "initial speed";
    frame_initspeed = uicontrol(frame_posspeed, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "gridbag", ...
    "border", b_posspeed);

    //speed and direction in initspeed
    c = createConstraints("gridbag", [1 1 1 1], [1 1], "horizontal", "center", [0 0], [0 20]);

    speed_frame = uicontrol(frame_initspeed, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "gridbag", ...
    "constraints", c);

    createSliderFrame(speed_frame, "speed", "speed", "(m/s)");

    c.grid =  [1 2 1 1]
    direction_frame = uicontrol(frame_initspeed, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "gridbag", ...
    "constraints", c);

    createSliderFrame(direction_frame, "dir", "dir", "(deg)");
    //Frame initpositiono
    //titled starting point
    //vertical grid: r_frame | 10 | theta_frame
    b_posspeed.title = "initial position";
    frame_initposition = uicontrol(frame_posspeed, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "gridbag", ...
    "border", b_posspeed);

    c.grid = [1 1 1 1]
    r_frame = uicontrol(frame_initposition, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "gridbag", ...
    "constraints", c);

    createSliderFrame(r_frame, "r", "r", "(m)");

    c.grid = [1 2 1 1]
    theta_frame = uicontrol(frame_initposition, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "gridbag", ...
    "constraints", c);

    createSliderFrame(theta_frame, "theta", "theta", "(deg)");

    //Button frame
    //esthetic | button 1 | est | button2 | est | button 3 | est
    gen_c.position = "bottom"
    gen_c.preferredsize = [0 30];
    button_frame = uicontrol(gui_frame, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "layout", "gridbag", ...
    "constraints", gen_c);

    // Buttons
    // est | buttons | est
    // =========================================================================
    btn_width = 100;
    btn_margin = 50;
    btn_height = 20;
    x = frame_x + 80

    c_button = createConstraints("gridbag", [1 1 1 1], [0 0], "none", "left", [0 0], [100 0]);

    //Esthetic Empty panel
    est_panel = uicontrol(button_frame, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "constraints", c_button);

    //Buttons
    buttonBorder = createBorder("line", "darkgrey", 3);
    c_button = createConstraints("gridbag", [2 1 1 1], [0 0], "both", "left", [0 0])
    //Button Start
    c_button.anchor = "left"
    start_button = uicontrol(button_frame, ...
    "Style", "pushbutton", ...
    "String", "Start", ...
    "Fontsize", 14, ...
    "BackgroundColor", [0.9 0.9 0.9], ...
    "callback", "start_simu()", ...
    "border", buttonBorder, ...
    "tag", "start_button", ...
    "constraints", c_button);

    //Empty panel
    c_button = createConstraints("gridbag", [3 1 1 1], [0 0], "none", "center", [0 0], [20 0])
    est_panel = uicontrol(button_frame, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "constraints", c_button);

    //Button Stop
    c_button = createConstraints("gridbag", [4 1 1 1], [0 0], "both", "center", [0 0])
    stop_button = uicontrol(button_frame, ...
    "Style", "pushbutton", ...
    "String", "Stop", ...
    "Fontsize", 14, ...
    "BackgroundColor", [0.9 0.9 0.9], ...
    "border", buttonBorder, ...
    "callback", "stop_simu", ...
    "tag", "stop_button", ...
    "constraints", c_button);

    //Empty panel
    c_button = createConstraints("gridbag", [5 1 1 1], [0 0], "none", "center", [0 0], [20 0])
    est_panel = uicontrol(button_frame, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "constraints", c_button);

    //Button Clear
    c_button = createConstraints("gridbag", [6 1 1 1], [0 0], "both", "center", [0 0])
    clear_button = uicontrol(button_frame, ...
    "Style", "pushbutton", ...
    "String", "Clear", ...
    "Fontsize", 14, ...
    "BackgroundColor", [0.9 0.9 0.9], ...
    "callback", "clear_simu", ...
    "border", buttonBorder, ...
    "tag", "clear_button", ...
    "constraints", c_button);

    //Empty panel
    c_button = createConstraints("gridbag", [7 1 1 1], [0 0], "none", "right", [100 0]);
    est_panel = uicontrol(button_frame, ...
    "style", "frame", ...
    "backgroundcolor", [1 1 1], ...
    "constraints", c_button);

    return
endfunction

// =============================================================================
// The callbacks
// =============================================================================
function change_r(r)
    //r slider callback
    // r is in [0.08 2]
    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");
    slider_r     = get("slider_r");

    value_r       = get("value_r");
    if argn(2)==1 then
        slider_r.Value=(r-0.007)/(2-0.08)*100;
    else
        r=slider_r.Value
        r=0.007+r*(2-0.08)/100
    end
    value_r.String=msprintf("%.3f", r)
    draw_initial_point(0.007+slider_r.Value*(2-0.08)/100, slider_theta.value*360/100, ...
    slider_speed.Value*3/100, slider_dir.value*360/100, %F);
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
    draw_initial_point(0.007+slider_r.Value*(2-0.08)/100, slider_theta.value*360/100, ..
    slider_speed.Value*3/100, slider_dir.value*360/100, %F);
endfunction

function change_speed(speed)
    //speed slider callback
    //speed is in [0 3]
    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");

    value_speed  = get("value_speed");
    if argn(2)==1 then
        slider_speed.Value=(speed)*100/3;
    else
        speed=slider_speed.Value
        speed=speed*3/100
    end
    value_speed.String=msprintf("%.0f", speed)
    draw_initial_point(0.007+slider_r.Value*(2-0.08)/100, slider_theta.value*360/100, ..
    slider_speed.Value*3/100, slider_dir.value*360/100, %F);
endfunction

function change_dir(dir)
    //direction slider callback
    //dir is in [0 360]
    slider_r        = get("slider_r");
    slider_theta    = get("slider_theta");
    slider_speed    = get("slider_speed");
    slider_dir      = get("slider_dir");
    value_dir       = get("value_dir");

    if argn(2) == 1 then
        slider_dir.Value = dir * 100 / 360;
    else
        dir = slider_dir.Value
        dir = dir * 360 / 100
    end

    value_dir.String = msprintf("%.0f", dir)
    draw_initial_point(0.007 + slider_r.Value * (2 - 0.08) / 100, slider_theta.value * 360 / 100, ...
    slider_speed.Value * 3 / 100, slider_dir.value * 360 / 100, %F);
endfunction

function start_simu()
    //start button callback
    my_figure_handle = get("main_figure");
    fin=my_figure_handle.user_data
    my_figure_handle.user_data=%f
    set("clear_button", "userdata", %f);
    slider_r     = get("slider_r");
    slider_theta = get("slider_theta");
    slider_speed = get("slider_speed");
    slider_dir   = get("slider_dir");
    t     = 0:0.02:15;
    Y = calculate_traj(0.007 + slider_r.Value * (2 - 0.08) / 100, slider_theta.value * 360 / 100, ...
    slider_speed.Value * 3 / 100, slider_dir.value * 360 / 100, t)
    x = Y(1, :)
    y = Y(3, :)
    r = 0.1; //bias to have the curve above the surface
    z = -1.0 ./ sqrt(x .^ 2 + y .^ 2) + r;
    curAxe = gca();
    traj_handle = curAxe.children(1).children(5);
    traj_handle.data = [x(1) y(1) z(1)];

    for k=2:size(x, "*")
        sleep(10)
        if is_handle_valid(my_figure_handle) then
            if execstr("fin = my_figure_handle.user_data", "errcatch") <> 0 | fin then break, end
            if is_handle_valid(traj_handle) then
                traj_handle.data = [traj_handle.data ; [x(k) y(k) z(k)]];
            else
                break;
            end
        else
            break;
        end
    end

    if get("clear_button", "userdata") then
        clear_simu()
    end

    if is_handle_valid(my_figure_handle) then
        my_figure_handle.user_data = %t;
    end
endfunction

function stop_simu()
    set("main_figure", "userdata", %t);
    set("clear_button", "userdata", %f);
endfunction

function clear_simu()
    //clear button callback
    my_figure_handle = get("main_figure");
    fin = my_figure_handle.user_data
    if fin then
        curAxe = gca();
        traj_handle = curAxe.children(1).children(5);
        traj_handle.data = [0 0 0];
        return
    end
    set("main_figure", "userdata", %t);
    set("clear_button", "userdata", %t);
endfunction
