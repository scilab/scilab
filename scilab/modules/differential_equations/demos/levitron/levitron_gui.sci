//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge Steer <Serge.Steer@scilab.org>
//
// This file is distributed under the same license as the Scilab package.
//

prot = funcprot();
funcprot(0);

//The graphics objects builder and subsidiaries
//--------------------------------------------
function [H]=build_levitron(h)
    //construct the graphical object representing the levitron in 3D

    n=21;// assumed to be odd
    [X, Y, Z]=levitron_facets(levitron_profile, n)
    N=n-1;
    np=1+size(X, 2)/N
    f = get("levitron_main_figure")

    //Plot the levitron
    f.color_map = graycolormap(N);
    f.immediate_drawing = "off"

    s=[((N/2)):(N-1), N:-1:((N/2)+1)];
    plot3d(X, Y, list(Z, ones(1, np-1).*.s))// the top

    H=gce();
    H.hiddencolor=-1;
    H.color_mode = -1;
    H.color_flag = 3;
    H.clip_state = "off";

    H.user_data=[0 0 0 0 0 0];
    //The z axis
    x=0:0.05:2.5;
    param3d(zeros(x), zeros(x), x) //z axis
    e=gce();e.thickness=2;
    //the bottom
    plot3d([-1 1]*1.5, [-1 1]*1.5, zeros(2, 2))
    e=gce();e.color_mode=color("red");

    isoview()
    levitron_axes = gca();
    //Axes definition for the plot
    //plot will be on the left-hand side of the screen
    levitron_axes.axes_bounds = [0 0 3.5/5 1];
    levitron_axes.tight_limits="on";
    levitron_axes.clip_state="off";
    levitron_axes.data_bounds=[-1 -1 0;1 1 2];
    levitron_axes.axes_visible="off";
    levitron_axes.box="off";
    levitron_axes.cube_scaling="on";
    levitron_axes.rotation_angles=[76 45];
    levitron_axes.foreground=N
    levitron_axes.x_label.text=""
    levitron_axes.y_label.text=""
    levitron_axes.z_label.text=""

    f.immediate_drawing ="on"
    f.visible = "on"
endfunction

function [zp, ro]=levitron_profile()
    //zp : elevation of the profile's points
    //ro : radius
    //c  : color
    zp = [0.6;0.22;0.15;0.06;0.04;0.02;-0.04;-0.06;
    -0.07;-0.15;-0.19;-0.27;-0.34;-0.38]*0.8;
    ro = [0;0.03;0.07;0.42;0.43;0.44;0.44;
    0.43;0.42;0.07;0.05;0.03;0.02;0];
    c = ones(ro)
endfunction

function [X, Y, Z]=levitron_facets(prof, n)
    //computes the facets of the surface of a revolution surface
    // prof is a function that returns the elevation and the radiud of ech
    // profile point
    // n: is the number of points for the rotation angle discretization

    [zp, ro]=prof();//one profile
    np=size(ro, "*");
    //generate points on the top surface
    t=linspace(0, 2*%pi, n);
    x=ro*cos(t);//np x n
    y=ro*sin(t);
    z=zp*ones(t);

    //build the facets
    Iz=([0;0;1;1]*ones(1, np-1)+ones(4, 1)*(1:(np-1))).*.ones(1, n-1);
    It=ones(1, np-1).*.([0;1;1;0]*ones(1, n-1)+ones(4, 1)*(0:(n-2)));
    I=matrix(Iz+np*It, -1, 1);
    X=matrix(x(I), 4, -1);
    Y=matrix(y(I), 4, -1);
    Z=matrix(z(I), 4, -1);
endfunction


//The callbacks
//-------------------------------------------------------------

function update_height(h)
    Slider_Height=get("slider_height");
    Value_Height =get("value_height")
    if argn(2)==1 then
        Slider_Height.Value=(h-1.6)*100/0.2;
    else
        h=Slider_Height.Value
        h=max(0.001, 1.6+h*0.2/100)
    end
    Value_Height.String=msprintf("%.3f", h)
    update_state(3, h)
endfunction

function update_theta(t)
    Slider_Theta=get("slider_theta")
    Value_Theta=get("value_theta")
    if argn(2)==1 then
        Slider_Theta.Value=(t-0.01)*100;
    else
        t=Slider_Theta.Value
        t=t/100 + 0.01
    end
    Value_Theta.String=msprintf("%.3f", t)
    update_state(4, t)
endfunction

function update_phi(p)
    Slider_Phi=get("slider_phi")
    Value_Phi=get("value_phi")
    if argn(2)==1 then
        Slider_Phi.Value=p*100/360;
    else
        p=Slider_Phi.Value
        p=360*p/100
    end
    Value_Phi.String=msprintf("%.3f", p)
    update_state(5, p)
endfunction

function update_psi(p)
    Slider_Psi=get("slider_psi")
    Value_Psi=get("value_psi")
    if argn(2)==1 then
        Slider_Psi.Value=p*100/360;
    else
        p=Slider_Psi.Value
        p=360*p/100
    end
    Value_Psi.String=msprintf("%.3f", p)
    update_state(6, p)
endfunction


function levitron_start()
    set("Stop", "userdata", %f);
    set("Reinit", "userdata", %f);
    show();
endfunction

function levitron_stop()
    set("Stop", "userdata", %t);
    set("Reinit", "userdata", %f);
endfunction

function levitron_reinit()
    global y0 y1
    set("Stop", "userdata", %t);
    set("Reinit", "userdata", %t);
    y1=y0
    update_height(1.72)
    update_theta(0.28)
    update_phi(0)
    update_psi(0)
endfunction


// Callbacks subsidiary functions
//-------------------------------
function update_state(k, value)
    global y1 state_changed
    if get("Stop", "userdata") then
        if or(k==(4:6)) then value=value*%pi/180;end
        y1(k)=value;
        if get("Reinit", "userdata") then
            set_levitron(H, y1)
        else
            state_changed=%t;
        end
    end
endfunction

function set_levitron(H, q)
    O=H.user_data;
    Data=H.data;
    f=20 //Dilatation factor of X and Y coordinates
    XYZ=[matrix(Data.x-f*O(1), 1, -1)
    matrix(Data.y-f*O(2), 1, -1)
    matrix(Data.z-O(3), 1, -1)];
    psi  =q(6); //precession:rotation Oxyz ->Ouvz
    theta=q(4);//nutation Ouvz -> Ouwz'
    phi  =q(5);// rotation propre Ouwz'->0x'y'z' (referentiel attache a la toupie)

    XYZ=euler(psi, theta, phi)*eulerm1(O(6), O(4), O(5))*XYZ

    drawlater()
    Data.x=f*q(1)+matrix(XYZ(1, :), 4, -1);
    Data.y=f*q(2)+matrix(XYZ(2, :), 4, -1);
    Data.z=q(3)+matrix(XYZ(3, :), 4, -1);
    H.user_data(1:6)=q(1:6)';
    H.data=Data;
    drawnow()
endfunction

function show()
    global y1 state_changed
    //y1 is a copy of that can be updated by the gui (state_changed is true
    //if the GUI has modified its value).
    y=y1;
    dt=0.05
    [y, w, iw]=ode(y, 0, dt, list(levitron_dyn, a, c, Mc)); y1=y;
    realtimeinit(dt)
    t0=dt
    k=1
    while ~get("Stop", "userdata")
        if state_changed then
            [y, w, iw]=ode(y1, t0, t0+dt, list(levitron_dyn, a, c, Mc));
        else
            [y, w, iw]=ode(y, t0, t0+dt, list(levitron_dyn, a, c, Mc), w, iw);
        end
        y1=y;
        if y(3)<=0 then set("Stop", "userdata", %t), break; end
        if get("Stop", "userdata")
            break
        end
        if ~is_handle_valid(H) then break; end
        set_levitron(H, y(1:6));
        update_height(y(3))
        update_theta(y(4)*180/%pi)
        update_phi(modulo(y(5)*180/%pi, 360))
        update_psi(modulo(y(6)*180/%pi, 360))

        k=k+1;t0=t0+dt;
        realtime(k);
    end
    if get("Reinit", "userdata") then
        levitron_reinit()
    end
endfunction

function levitron_create_gui()
    //Simulation parameters
    height_param = tlist(["sim_param", "name", "unit", "default"]);
    height_param.name = "height";
    height_param.unit = "m";
    height_param.default = (1.72-1.6)*50;

    theta_param  = tlist(["sim_param", "name", "unit", "default"]);
    theta_param.name = "theta"
    theta_param.unit = "deg"
    theta_param.default = 28;

    phi_param    = tlist(["sim_param", "name", "unit", "default"]);
    phi_param.name = "phi"
    phi_param.unit = "deg"
    phi_param.default = 0;

    psi_param    = tlist(["sim_param", "name", "unit", "default"]);
    psi_param.name = "psi"
    psi_param.unit = "deg"
    psi_param.default = 0

    param_list = list(height_param, theta_param, phi_param, psi_param);

    //Create the figure and the frame to put the sliders
    levitron_main_fig = figure( ...
    "dockable", "off", ...
    "infobar_visible", "off", ...
    "toolbar_visible", "off", ...
    "toolbar", "none", ...
    "menubar_visible", "off", ...
    "menubar", "none", ...
    "default_axes", "on", ...
    "layout", "none", ...
    "visible", "off", ...
    "immediate_drawing", "off", ...
    "background", -2, ...
    "figure_position", [0 0], ...
    "axes_size", [800, 600], ...
    "figure_name", "Levitron Simulation", ...
    "tag", "levitron_main_figure");


    levitron_main_fig.layout = "gridbag"

    //An empty frame to push parameters on the right when resizing
    c = createConstraints("gridbag", [1, 1, 1, 1], [0.75, 1], "horizontal", "upper_left", [0, 0], [500, 0])
    empty_frame = uicontrol(levitron_main_fig, "style", "frame", "constraints", c, "backgroundcolor", [1, 0, 0]);

    c = createConstraints("gridbag", [2, 1, 1, 1], [0.25, 1], "both", "right", [0, 0], [300, 0]);
    right_frame = uicontrol(levitron_main_fig, "style", "frame", "backgroundcolor", [1, 1, 1], ...
    "constraints", c);

    right_frame.layout = "border";
    right_frame.layout_options = createLayoutOptions("border");

    l_border = createBorder("line", "black", 2)
    param_border = createBorder("titled", l_border, "Simulation Parameters", "center", "top")
    c = createConstraints("border", "center");
    param_frame = uicontrol(right_frame, "style", "frame", "backgroundcolor", [1, 1, 1], ...
    "constraints", c, "tag", "param_frame", "border", param_border, "layout", "gridbag");

    c = createConstraints("border", "bottom", [0, 50]);
    button_frame = uicontrol(right_frame, "style", "frame", "backgroundcolor", [1, 1, 1], ...
    "constraints", c, "tag", "button_frame", "layout", "gridbag");

    c = createConstraints("border", "right", [10, 0]);
    empty_frame = uicontrol(right_frame, "style", "frame", "backgroundcolor", [1, 1, 1], ...
    "constraints", c);

    c = createConstraints("border", "left", [10, 0]);
    empty_frame = uicontrol(right_frame, "style", "frame", "backgroundcolor", [1, 1, 1], ...
    "constraints", c);

    gui_createParamFrame(param_list);
    gui_createButtonFrame();
endfunction

function gui_createParamFrame(param_list);
    param_frame = get("param_frame");

    for i = 1:size(param_list)
        c = createConstraints("gridbag", [1, i, 1, 1], [1, 1], "both", "left", [5, 0], [25, 0]);
        name_p = uicontrol(param_frame, ...
        "Style", "text", ...
        "String", param_list(i).name + " ", ...
        "BackgroundColor", [1 1 1], ...
        "horizontalalignment", "right", ...
        "constraints", c, ...
        "tag", "text_" + param_list(i).name);

        c = createConstraints("gridbag", [2, i, 1, 1], [1, 1], "both", "center", [0, 0], [60, 0]);
        slider_p = uicontrol(param_frame, ...
        "Style", "slider", ...
        "Value", param_list(i).default, ...
        "Min", 0, ...
        "Max", 100, ...
        "callback", "update_" + param_list(i).name + "()", ...
        "constraints", c, ...
        "horizontalalignment", "center", ...
        "backgroundcolor", [1, 1, 1], ...
        "tag", "slider_" + param_list(i).name);

        c = createConstraints("gridbag", [3, i, 1, 1], [1, 1], "both", "left", [15, 0], [25, 0]);
        value_p = uicontrol(param_frame, ...
        "Style", "text", ...
        "String", msprintf("%.3f", param_list(i).default), ...
        "BackgroundColor", [1 1 1], ...
        "constraints", c, ...
        "foregroundcolor", [0 0 1], ...
        "horizontalalignment", "right", ...
        "tag", "value_" + param_list(i).name);

        c = createConstraints("gridbag", [4, i, 1, 1], [1, 1], "both", "right", [5, 0], [25, 0]);
        unite_p = uicontrol(param_frame, ...
        "Style", "text", ...
        "String", " " + param_list(i).unit, ...
        "BackgroundColor", [1 1 1], ...
        "constraints", c, ...
        "horizontalalignment", "left", ...
        "tag", "unite_" + param_list(i).name);
    end
endfunction


function gui_createButtonFrame();
    button_frame = get("button_frame");

    c = createConstraints("gridbag", [1, 1, 1, 1], [1, 1], "horizontal", "left");
    empty = uicontrol(button_frame, "style", "frame", "backgroundcolor", [1, 1, 1], "constraints", c);

    c = createConstraints("gridbag", [2, 1, 1, 1], [0, 0], "none", "center");
    Start = uicontrol("parent", button_frame, ...
    "Style"     , "pushbutton", ...
    "String"    , "Start", ...
    "callback"  , "levitron_start()", ...
    "constraints", c, ...
    "tag", "Start");

    c = createConstraints("gridbag", [3, 1, 1, 1], [1, 1], "horizontal", "center");
    empty = uicontrol(button_frame, "style", "frame", "backgroundcolor", [1, 1, 1], "constraints", c);

    c = createConstraints("gridbag", [4, 1, 1, 1], [0, 0], "none", "center");
    Stop = uicontrol("parent", button_frame, ...
    "Style"     , "pushbutton", ...
    "String"    , "Stop", ...
    "callback"  , "levitron_stop()", ...
    "callback_type", 10, ...
    "constraints", c, ...
    "tag", "Stop");

    c = createConstraints("gridbag", [5, 1, 1, 1], [1, 1], "horizontal", "center");
    empty = uicontrol(button_frame, "style", "frame", "backgroundcolor", [1, 1, 1], "constraints", c);

    c = createConstraints("gridbag", [6, 1, 1, 1], [0, 0], "none", "center");
    Reinit = uicontrol("parent", button_frame, ...
    "Style"     , "pushbutton", ...
    "String"    , "Reset", ...
    "callback"  , "levitron_reinit()", ...
    "callback_type", 10, ...
    "constraints", c, ...
    "tag", "Reinit");

    c = createConstraints("gridbag", [7, 1, 1, 1], [1, 1], "horizontal", "right");
    empty = uicontrol(button_frame, "style", "frame", "backgroundcolor", [1, 1, 1], "constraints", c);
endfunction

funcprot(prot);
