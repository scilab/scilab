function importgui_preview()
    global %importgui_cancel;
    //get window information
    delim = get("importgui_delim", "value")
    if delim == 1 then
        set("importgui_decimal", "value", 1);
    end

    set("importgui_btnvarialble", "enable", "off");
    set("importgui_btnfunction", "enable", "off");
    
    hasHeader = get("importgui_header", "value");
    delim = get("importgui_delim", "userdata")(delim);
    decimal = get("importgui_decimal", "userdata")(get("importgui_decimal", "value"));
    conversion = get("importgui_conversion", "userdata")(get("importgui_conversion", "value"));

    path = get("importgui_filename", "string");
    
    if isempty(path) || ~isfile(path) then
        return;
    end
    
    c = get("importgui_preview");
    c.visible = "off";
    delete(c.children);

    parent = c.parent;
    fr = uicontrol(parent, ...
        "style", "frame", ...
        "layout", "gridbag", ...
        "tag", "importgui_cancel", ...
        "constraints", createConstraints("border", "center"));

    uicontrol(fr, ...
        "style", "pushbutton", ...
        "string", _("Cancel preview"), ...
        "callback_type", 10, ...
        "callback", "global %importgui_cancel;%importgui_cancel=%t;", ...
        "constraints", createConstraints("gridbag", [1 1 1 1], [1, 1]));

    txt = mgetl(path, 20 + hasHeader);
    if hasHeader then
        header = txt(1);
        txt(1) = [];
        
        //process header
        try
            header = csvTextScan(header, delim, decimal, "string");
        catch
            delete(fr);
            c.visible = "on";
            return;
        end
    end
    
    try
        x = csvTextScan(txt, delim, decimal, conversion);
    catch
        delete(fr);
        c.visible = "on";
        return;
    end
    
    if conversion == "double" then
        //must convert double to string to display data
        x = string(x);
    end
    
    //frame data
    limit = min(10, size(x, "c"));
    show = get("importgui_showcol", "value");
    if show then
        limit = size(x, "c");
    end

    for i = 1:limit
        if %importgui_cancel then
            break;
        end
        
        progress = i / limit;
        constraints = createConstraints("gridbag", [i 1 1 1], [1, 0], "horizontal", "upper", [5, 5]);
       
        if hasHeader then
            h = header(i);
        else
            h = sprintf("col%d", i);
        end
        
        uicontrol(c, ...
            "style", "text", ...
            "string", h, ...
            "horizontalalignment", "center", ...
            "relief", "solid", ...
            "constraints", constraints)
        
        for j = 1:size(x, "r")
            constraints = createConstraints("gridbag", [i j+1 1 1], [1, 0], "horizontal", "upper", [7 7]);
            
            uicontrol(c, ...
                "style", "text", ...
                "horizontalalignment", "center", ...
                "string", x(j,i), ...
                "constraints", constraints);
        end
        
        importgui_progress(progress);
    end
    
    //to push lines to top
    constraints = createConstraints("gridbag", [1 size(x, "r")+1 size(x, "c") 1], [1, 1], "both", "upper");
    uicontrol(c, ...
        "style", "frame", ...
        "constraints", constraints);
            
    //delete cancel frame        
    delete(fr);
    
    if %importgui_cancel then
        //delete created children to keep interface clean
        delete(c.children)
    else
        set("importgui_btnvarialble", "enable", "on");
        set("importgui_btnfunction", "enable", "on");
    end

    clearglobal %importgui_cancel;
    c.visible = "on";
    importgui_progress(0);
endfunction
