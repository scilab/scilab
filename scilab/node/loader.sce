if 0 then
    f1 = createWindow();
    f1.position(3:4) = [250, 100];

    uicontrol(f1, "position", [25 25 50 50], "string", "+1", "callback", "v = eval(get(''num'', ''string''));set(''num'', ''string'', string(v + 1));");
    uicontrol(f1, "position", [100 25 50 50], "string", "0", "tag", "num", "callback", "set(''num'', ''string'', ''0'');");
    uicontrol(f1, "position", [175 25 50 50], "string", "-1", "callback", "v = eval(get(''num'', ''string''));set(''num'', ''string'', string(v - 1));");
end

if 0 then
    f2 = createWindow();
    f2.visible = %f;
    f2.position(3:4) = [500, 600];
    for i=1:81
        x = floor((i-1) / 9);
        y = modulo((i-1), 9);
        uicontrol(f2, "position", [25 + 50*x, 125 + 50*y,50,50],"string", string(x) + " - " + string(y), "callback", "set(gcbo, ''enable'', %f)");
    end
    
    uicontrol(f2, "position", [200, 25, 100, 50], "string", "reset", "callback", "c = gcbo.parent.children;for i=1:size(c, ''*''), c(i).enable = %t;end");
    f2.visible = %t;
end

if 0 then
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


if 0 then
    f = createWindow();
    f.position = [200 200 240 470];
    f.figure_name = "";

    fr1 = uicontrol("style", "frame", "position", [10 10 220 450], "border", createBorder("etched"));
    spinner = uicontrol(fr1, "style", "spinner", "string", "0", "position", [10 410 200 30], "horizontalalignment", "right", "min", 0, "max", 5, "backgroundcolor", [0 1 0]);
    label = uicontrol(fr1, "style", "text", "string", "usedeprecatedskin=""off""", "position", [10 370 200 30], "backgroundcolor", [1 0 1]);
    editbox = uicontrol(fr1, "style", "edit", "string", "Edit", "position", [10 330 200 30], "backgroundcolor", [1 0 0]);
    button = uicontrol(fr1, "style", "pushbutton", "string", "Pushbutton", "position", [10 290 200 30]);
    checkbox = uicontrol(fr1, "style", "checkbox", "string", "Checkbox", "position", [10 250 200 30], "backgroundcolor", [0 0 1]);
    radio = uicontrol(fr1, "style", "radiobutton", "string", "Radiobutton", "position", [10 210 200 30], "backgroundcolor", [1 0 1]);
    slider = uicontrol(fr1, "style", "slider", "position", [10 150 200 50]);
    frame = uicontrol(fr1, "style", "frame", "position", [10 110 200 30], "backgroundcolor", [0.5 0.5 0.5]);
    listbox = uicontrol(fr1, "style", "listbox", "string", "listbox1|listbox2|listbox3|listbox4|listbox5", "position", [10 50 200 50], "backgroundcolor", [0.2 0.2 0.2]);
    combo = uicontrol(fr1, "style", "popupmenu", "string", "popupmenu1|popupmenu2", "position", [10 10 200 30], "backgroundcolor", [0.8 0.8 0.8]);
end

if 0 then
    xdel(winsid());
    clear;

    function onchange()
        R = get("sliderR", "value") / 255;
        G = get("sliderG", "value") / 255;
        B = get("sliderB", "value") / 255;
        set("color", "backgroundcolor", [R, G, B]);
    endfunction
    

    f = createWindow();
    f.visible = %f;
    f.position(3:4) = [396, 130];

    uicontrol("style", "slider", "position", [10 10 256 30], "min", 0, "max", 255, "sliderstep", [1, 1], "value", 0, "tag", "sliderB");
    uicontrol("style", "slider", "position", [10 50 256 30], "min", 0, "max", 255, "sliderstep", [1, 1], "value", 0, "tag", "sliderG");
    uicontrol("style", "slider", "position", [10 90 256 30], "min", 0, "max", 255, "sliderstep", [1, 1], "value", 0, "tag", "sliderR");
    
    uicontrol("style", "frame", "position", [286 20 90 90], "tag", "color", "backgroundcolor", [0, 0, 0]);
    
    set("sliderR", "callback", "onchange");
    set("sliderG", "callback", "onchange");
    set("sliderB", "callback", "onchange");
    
    f.visible = %t;
end

//layout border
if 0 then
    
    f = createWindow();
    f.layout = "border";
    f.layout_options = createLayoutOptions("border", [10, 10]);
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

//gridbag
if 1 then
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
