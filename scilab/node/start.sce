//1 +/-
//2 damier
//3 spinner and sliders
//4 demo uicontrol
//5 RGB
//6 RGB two windows
//7 border layout
//8 gridbag layout
//9 tab
//0 visible
//11 opticlim uicontrol
//12 normalized
//13 plot 2d

//100 opticlim

demo = 13;

if demo == 1 then
    f1 = createWindow();
    f1.position(3:4) = [250, 100];

    uicontrol(f1, "position", [25 25 50 50], "string", "+1", "callback", "v = eval(get(''num'', ''string''));set(''num'', ''string'', string(v + 1));");
    uicontrol(f1, "position", [100 25 50 50], "string", "0", "tag", "num", "callback", "set(''num'', ''string'', ''0'');");
    uicontrol(f1, "position", [175 25 50 50], "string", "-1", "callback", "v = eval(get(''num'', ''string''));set(''num'', ''string'', string(v - 1));");
end

if demo == 2 then
    dim = 5;
    f2 = createWindow();
    f2.visible = %f;
    f2.position(3:4) = [50 * dim + 50, 50 * dim + 125];
    for i=1:dim*dim
        x = floor((i-1) / dim);
        y = modulo((i-1), dim);
        uicontrol(f2, "position", [25 + 50*x, 100 + 50*y,50,50],"string", string(x) + " - " + string(y), "callback", "set(gcbo, ''enable'', %f)");
    end

    uicontrol(f2, "position", [f2.position(3)/2 - 50, 25, 100, 50], "string", "reset", "callback", "c = gcbo.parent.children;for i=1:size(c, ''*''), c(i).enable = %t;end");
    f2.visible = %t;
end

if demo == 3 then
    xdel(winsid());
    clear;

    function reset()
        minval = get("min", "value");
        maxval = get("max", "value");
        set("slider", "value", string(((maxval - minval) / 2) + minval));
    endfunction

    function onchange()
        set("slider", "min", get("min", "value"));
        step = get("step", "value");
        set("slider", "sliderstep", [step, step*10]);
        set("slider", "max", get("max", "value"));
    endfunction

    f = createWindow();
    f.visible = %f;
    f.position(3:4) = [300, 200];

    uicontrol("style", "text", "string", "0", "position", [120, 160, 60, 20], "horizontalalignment", "center", "tag", "val");
    uicontrol("style", "slider", "position", [10 120 280 30], "min", -100, "max", 100, "sliderstep", [1, 1], "value", 0, "tag", "slider");

    uicontrol("style", "text", "string", "min", "position", [25 80 50 20], "horizontalalignment", "center");
    uicontrol("style", "text", "string", "step", "position", [125 80 50 20], "horizontalalignment", "center");
    uicontrol("style", "text", "string", "max", "position", [225 80 50 20], "horizontalalignment", "center");

    uicontrol("style", "spinner", "min", -100, "max", 0, "sliderstep", [1, 1], "value", "-100", "position", [25 50 50 20], "horizontalalignment", "center", "tag", "min");
    uicontrol("style", "spinner", "min", 1, "max", 10, "sliderstep", [1, 1], "value", "1", "position", [125 50 50 20], "horizontalalignment", "center", "tag", "step");
    uicontrol("style", "spinner", "min", 0, "max", 100, "sliderstep", [1, 1], "value", "100", "position", [225 50 50 20], "horizontalalignment", "center", "tag", "max");

    uicontrol("style", "pushbutton", "string", "reset", "position", [100 10 100 30], "callback", "reset");
    f.visible = %t;

    set("slider", "callback", "val = gcbo.value;set(""val"", ""string"", string(val));");
    set("min", "callback", "onchange");
    set("step", "callback", "onchange");
    set("max", "callback", "onchange");

end


if demo == 4 then
    f = createWindow();
    f.position = [200 200 240 470];
    f.figure_name = "";

    fr1 = uicontrol("style", "frame", "position", [10 10 220 450], "border", createBorder("etched"));
    spinner = uicontrol(fr1, "style", "spinner", "string", "0", "position", [10 410 200 30], "horizontalalignment", "right", "min", 0, "max", 5);
    label = uicontrol(fr1, "style", "text", "string", "usedeprecatedskin=""off""", "position", [10 370 200 30], "backgroundcolor", [1 0 1]);
    editbox = uicontrol(fr1, "style", "edit", "string", "Edit", "position", [10 330 200 30], "backgroundcolor", [1 0 0]);
    button = uicontrol(fr1, "style", "pushbutton", "string", "Pushbutton", "position", [10 290 200 30]);
    checkbox = uicontrol(fr1, "style", "checkbox", "string", "Checkbox", "position", [10 250 200 30]);
    radio = uicontrol(fr1, "style", "radiobutton", "string", "Radiobutton", "position", [10 210 200 30]);
    slider = uicontrol(fr1, "style", "slider", "position", [10 150 200 50]);
    frame = uicontrol(fr1, "style", "frame", "position", [10 110 200 30], "backgroundcolor", [0.5 0.5 0.5]);
    listbox = uicontrol(fr1, "style", "listbox", "string", "listbox1|listbox2|listbox3|listbox4|listbox5", "position", [10 50 200 50], "backgroundcolor", [0.2 0.2 0.2]);
    combo = uicontrol(fr1, "style", "popupmenu", "string", "popupmenu1|popupmenu2", "position", [10 10 200 30], "backgroundcolor", [0.8 0.8 0.8]);
end

if demo == 5 then
    xdel(winsid());
    clear onChange;

    function onChange()
        R = get("sliderR", "value");
        G = get("sliderG", "value");
        B = get("sliderB", "value");
        set("textR", "string", string(R));
        set("textG", "string", string(G));
        set("textB", "string", string(B));
        set("color", "backgroundcolor", [R, G, B]/255);
    endfunction


    f = createWindow();
    f.visible = %f;
    f.position(3:4) = [300, 130];

    uicontrol("style", "slider", "position", [10 10 128 30], "min", 0, "max", 255, "sliderstep", [1, 10], "value", 0, "tag", "sliderB");
    uicontrol("style", "slider", "position", [10 50 128 30], "min", 0, "max", 255, "sliderstep", [1, 10], "value", 0, "tag", "sliderG");
    uicontrol("style", "slider", "position", [10 90 128 30], "min", 0, "max", 255, "sliderstep", [1, 10], "value", 0, "tag", "sliderR");

    uicontrol("style", "text", "position", [150 15 40 20], "string", "0", "backgroundcolor", [1, 1, 1], "horizontalalignment", "center", "tag", "textB");
    uicontrol("style", "text", "position", [150 55 40 20], "string", "0", "backgroundcolor", [1, 1, 1], "horizontalalignment", "center", "tag", "textG");
    uicontrol("style", "text", "position", [150 95 40 20], "string", "0", "backgroundcolor", [1, 1, 1], "horizontalalignment", "center", "tag", "textR");

    uicontrol("style", "frame", "position", [200 20 90 90], "tag", "color", "backgroundcolor", [0, 0, 0]);

    set("sliderR", "callback", "onChange");
    set("sliderG", "callback", "onChange");
    set("sliderB", "callback", "onChange");

    f.visible = %t;
end

if demo == 6 then
    xdel(winsid());
    clear onChange;
    function onChange()
        R = get("sliderR", "value");
        G = get("sliderG", "value");
        B = get("sliderB", "value");
        set("textR", "string", string(R));
        set("textG", "string", string(G));
        set("textB", "string", string(B));
        set("color", "backgroundcolor", [R, G, B]/255);
    endfunction


    f1 = createWindow();
    f1.position = [100, 100, 350, 130];
    f1.figure_name = "sliders";

    uicontrol(f1, "style", "slider", "position", [10 10 256 30], "min", 0, "max", 255, "sliderstep", [1, 10], "value", 0, "tag", "sliderB");
    uicontrol(f1, "style", "slider", "position", [10 50 256 30], "min", 0, "max", 255, "sliderstep", [1, 10], "value", 0, "tag", "sliderG");
    uicontrol(f1, "style", "slider", "position", [10 90 256 30], "min", 0, "max", 255, "sliderstep", [1, 10], "value", 0, "tag", "sliderR");

    uicontrol(f1, "style", "text", "position", [300 15 40 20], "string", "0", "backgroundcolor", [1, 1, 1], "horizontalalignment", "center", "tag", "textB");
    uicontrol(f1, "style", "text", "position", [300 55 40 20], "string", "0", "backgroundcolor", [1, 1, 1], "horizontalalignment", "center", "tag", "textG");
    uicontrol(f1, "style", "text", "position", [300 95 40 20], "string", "0", "backgroundcolor", [1, 1, 1], "horizontalalignment", "center", "tag", "textR");

    f2 = createWindow();
    f2.position = [450, 100, 130, 130];
    f2.figure_name = "RGB";
    uicontrol(f2, "style", "frame", "position", [15 15 100 100], "tag", "color", "backgroundcolor", [0, 0, 0]);

    set("sliderR", "callback", "onChange");
    set("sliderG", "callback", "onChange");
    set("sliderB", "callback", "onChange");
end

//border layout
if demo == 7 then

    f = createWindow();
    f.layout = "border";
    f.layout_options = createLayoutOptions("border", [10, 20]);
    //f.layout_options = createLayoutOptions("border");

    c = createConstraints("border", "top", [50,20]);

    t = %t;
    l = %t;
    m = %t;
    r = %t;
    b = %t;

    if t then
        u_grid1 = uicontrol(f,...
        "style", "frame",...
        "backgroundcolor", [1 0 0], ...
        "constraints", c);
    end

    if l then
        c.position = "left";
        u_grid2 = uicontrol(f,...
        "style", "frame",...
        "backgroundcolor", [0 1 0], ...
        "constraints", c);
    end

    if m then
        c.position = "center";
        u_grid3 = uicontrol(f,...
        "style", "frame",...
        "backgroundcolor", [0 0 1], ...
        "constraints", c);
    end

    if r then
        c.preferredsize = [100, 100];
        c.position = "right";
        u_grid4 = uicontrol(f,...
        "style", "frame",...
        "backgroundcolor", [1 1 0], ...
        "constraints", c);
    end

    if b then
        c.position = "bottom";
        u_grid5 = uicontrol(f,...
        "style", "frame",...
        "backgroundcolor", [1 0 1], ...
        "constraints", c);
    end
end

//gridbag layout
if demo == 8 then
    f = createWindow();
    f.position = [200 200 200 100];
    f.layout = "gridbag";

    c = createConstraints("gridbag",[1, 1, 1, 1], [1, 1], "both", "center", [0, 0], [50, 50]);

    redf_grid   = [1, 1, 1, 1];
    greenf_grid = [1, 2, 1, 1];
    bluef_grid  = [1, 3, 2, 1];
    yelf_grid   = [2, 1, 1, 2];
    magf_grid   = [3, 1, 1, 1];
    cyanf_grid  = [3, 2, 1, 2];

    c.grid = bluef_grid;
    u_grid3 = uicontrol(f , ...
    "style"               , "frame", ...
    "backgroundcolor"     , [0 0 1], ...
    "constraints"         , c);

    c.grid = cyanf_grid;
    u_grid1 = uicontrol(f , ...
    "style"               , "frame", ...
    "backgroundcolor"     , [0 1 1], ...
    "constraints"         , c);

    c.grid = redf_grid;
    u_grid1 = uicontrol(f , ...
    "style"               , "frame" , ...
    "backgroundcolor"     , [1 0 0] , ...
    "constraints"         , c);

    c.grid = greenf_grid;
    u_grid2 = uicontrol(f , ...
    "style"               , "frame", ...
    "backgroundcolor"     , [0 1 0], ...
    "constraints"         , c);

    c.grid = yelf_grid;
    u_grid4 = uicontrol(f , ...
    "style"               , "frame", ...
    "backgroundcolor"     , [1 1 0], ...
    "constraints"         , c);

    c.grid = magf_grid;
    u_grid5 = uicontrol(f , ...
    "style"               , "frame", ...
    "backgroundcolor"     , [1 0 1], ...
    "constraints"         , c);
end


//tab
if demo == 9 then

    xdel(winsid());
    clear onPush;
    function onPush()
        set("push", "string", string(get("tab", "value")));
    endfunction

    f = createWindow();
    f.position = [200 200 400 300];

    tab = uicontrol(f, "style", "tab", "tag", "tab", "position", [0 30 400, 270]);

    tab3 = uicontrol(tab, "style", "frame", "string", "Blue", "layout", "border", "backgroundcolor", [0 0 1]);
    tab2 = uicontrol(tab, "style", "frame", "string", "Green", "layout", "border", "backgroundcolor", [0 1 0]);
    tab1 = uicontrol(tab, "style", "frame", "string", "Red", "layout", "border", "backgroundcolor", [1 0 0]);

    uicontrol(f, "style", "pushbutton", "string", "push me", "callback", "onPush", "tag", "push", "position", [150 0 100 30]);

    set("tab", "value", 1);
end

if demo == 10 then

    fig_pregui = figure("figure_name", "OPTICLIM - site and weather selection", ...
    "dockable", "off", ...
    "axes_size", [500,170], ...
    "infobar_visible", "off", ...
    "toolbar", "none", ...
    "menubar_visible", "on", ...
    "menubar", "none", ...
    "default_axes", "off", ...
    "resize", "off", ...
    "layout", "border", ...
    "background", color(244,244,244), ...
    "tag", "figure_preopticlim", ...
    "visible", "off");

    //b1 = createBorder("titled", createBorder("etched"), "Choose your site:", "left", "top", createBorderFont("", 11, "bold"), "black");

    OpticlimPath = "E:\git\toolbox\sanofi-opticlim\";
    f = uicontrol(fig_pregui, "style", "frame", ...
    "fontsize", 11);
    //font1 = createBorderFont("", 11, "bold");
    //b1 = createBorder("titled", createBorder("etched"), _d("OPTICLIM", "Choose your site")+":", "left", "top", font1, "black");
    f1 = uicontrol(f, "style", "frame", ...
    "layout", "gridbag", ...
    "constraints", createConstraints("gridbag", [1 1 3 1], [1 1], "both", "left"), ...
    "fontsize", 11);
    uicontrol(f1, "style", "popupmenu", "string", [], ...
    "fontsize", 11, ...
    "horizontalAlignment", "center", ...
    "margins", [0,5,0,5], ...
    "tag", "site_list", ...
    "callback", "cbChangeSite", ...
    "constraints", createConstraints("gridbag", [1 1 1 1], [5 1], "horizontal", "left", [0,0], [330,22]));
    path_icon = OpticlimPath+"pictures/list-add.png";
    uicontrol(f1, "style", "pushbutton", "string", "", ...
    "fontsize", 11, ...
    "horizontalAlignment", "center", ...
    "icon", path_icon, ...
    "margins", [0,5,0,5], ...
    "callback", "cbAddSite", ...
    "constraints", createConstraints("gridbag", [2 1 1 1], [1 1], "horizontal", "left"));

    sleep(500);
    fig_pregui.visible = "on";
end

//opticlim uicontrol
if demo == 11 then
    xdel(winsid());
    clear onPush;
    function onPush()
        if 0 then
            set("tabs", "value", 2);
        end
        if 1 then
            r1 = "r1: " + string(get("radio1", "value")) + " ";
            r2 = "r2: " + string(get("radio2", "value")) + " ";
            c1 = "c1: " + string(get("check1", "value")) + " ";
            c2 = "c2: " + string(get("check2", "value")) + " ";
            t1 = "t1: " + string(get("tabs", "value"));
            if(t1 == [])
                t1 = "";
            end
            set("label", "string", r1+r2+c1+c2+t1);
        end
    endfunction
    f = createWindow();
    f.position = [200 200 240 460];
    f.figure_name = "";

    fr1 = uicontrol("style", "frame", "position", [10 10 220 440], "border", createBorder("etched"));
    checkbox1 = uicontrol(fr1, "style", "checkbox", "string", "Checkbox 1", "position", [10 400 200 30], "tag", "check1");
    checkbox2 = uicontrol(fr1, "style", "checkbox", "string", "Checkbox 2", "position", [10 370 200 30], "tag", "check2");
    editbox = uicontrol(fr1, "style", "edit", "string", "Edit", "position", [10 330 200 30]);
    frame = uicontrol(fr1, "style", "frame", "position", [10 270 200 50]);
    button = uicontrol(fr1, "style", "pushbutton", "string", "Pushbutton", "position", [10 230 200 30], "callback", "onPush");
    radio1 = uicontrol(fr1, "style", "radiobutton", "string", "Radio 1", "position", [10 190 200 30], "groupname", "radio", "tag", "radio1");
    radio2 = uicontrol(fr1, "style", "radiobutton", "string", "Radio 2", "position", [10 160 200 30], "groupname", "radio", "tag", "radio2");
    combo = uicontrol(fr1, "style", "popupmenu", "string", "popupmenu1|popupmenu2|popupmenu3|popupmenu4", "position", [10 120 200 30]);

    tab = uicontrol(fr1, "style", "tab", "position", [10 50 200 60], "tag", "tabs");
    tab3 = uicontrol(tab, "style", "frame", "string", "Blue", "backgroundcolor", [0 0 1]);
    tab2 = uicontrol(tab, "style", "frame", "string", "Green", "backgroundcolor", [0 1 0]);
    tab1 = uicontrol(tab, "style", "frame", "string", "Red", "backgroundcolor", [1 0 0]);

    label = uicontrol(fr1, "style", "text", "string", "some text", "position", [10 10 200 30], "horizontalalignment", "center", "tag", "label");
end

if demo == 12 then
    xdel(winsid());
    clear onPush;
    function onPush()
        r1 = "r1: " + string(get("radio1", "value")) + " ";
        r2 = "r2: " + string(get("radio2", "value")) + " ";
        c1 = "c1: " + string(get("check1", "value")) + " ";
        c2 = "c2: " + string(get("check2", "value")) + " ";
        t1 = "t1: " + string(get("tabs", "value"));
        if(t1 == [])
            t1 = "";
        end
        set("label", "string", r1+r2+c1+c2+t1);
    endfunction
    f = createWindow();
    f.position = [200 200 240 460];
    f.figure_name = "";

    fr1 = uicontrol(f, "style", "frame", "units", "normalized", "position", [0.02 0.02 0.96 0.96], "border", createBorder("etched"));

    checkbox1 = uicontrol(fr1, "style", "checkbox", "string", "Checkbox 1", "units", "normalized", "position", [0.02 0.9 0.96 0.09], "tag", "check1");
    checkbox2 = uicontrol(fr1, "style", "checkbox", "string", "Checkbox 2", "units", "normalized", "position", [0.02 0.8 0.96 0.09], "tag", "check2");
    editbox = uicontrol(fr1, "style", "edit", "string", "Edit", "units", "normalized", "position", [0.02 0.7 0.96 0.09]);
    button = uicontrol(fr1, "style", "pushbutton", "string", "Pushbutton", "units", "normalized", "position", [0.02 0.6 0.96 0.09], "callback", "onPush");
    radio1 = uicontrol(fr1, "style", "radiobutton", "string", "Radio 1", "units", "normalized", "position", [0.02 0.5 0.96 0.09], "groupname", "radio", "tag", "radio1");
    radio2 = uicontrol(fr1, "style", "radiobutton", "string", "Radio 2", "units", "normalized", "position", [0.02 0.4 0.96 0.09], "groupname", "radio", "tag", "radio2");
    combo = uicontrol(fr1, "style", "popupmenu", "string", "popupmenu1|popupmenu2|popupmenu3|popupmenu4", "units", "normalized", "position", [0.02 0.3 0.96 0.09]);

    tab = uicontrol(fr1, "style", "tab", "units", "normalized", "position", [0.02 0.1 0.96 0.195], "tag", "tabs");
    tab3 = uicontrol(tab, "style", "frame", "string", "Blue", "backgroundcolor", [0 0 1]);
    tab2 = uicontrol(tab, "style", "frame", "string", "Green", "backgroundcolor", [0 1 0]);
    tab1 = uicontrol(tab, "style", "frame", "string", "Red", "backgroundcolor", [1 0 0]);

    label = uicontrol(fr1, "style", "text", "string", "some text", "units", "normalized", "position", [0.02 0.005 0.96 0.09], "horizontalalignment", "center", "tag", "label");
end

if demo == 13 then
    xdel(winsid());
    f = createWindow();
    f.figure_name = "My first plot";
    f.position = [100, 100, 500, 400];

    fr = uicontrol(f, "style", "frame", "position", [0, 0, 500, 400]);
    newaxes(fr);
    plot(1:10);
end

//opticlim
if demo == 100 then
    exec("SCI/contrib/sanofi-opticlim/loader.sce");
    OpticlimStart();
end

