function importgui_function()

    hasHeader = get("importgui_header", "value");
    delim = get("importgui_delim", "userdata")(get("importgui_delim", "value"));
    decimal = get("importgui_decimal", "userdata")(get("importgui_decimal", "value"));
    conversion = get("importgui_conversion", "userdata")(get("importgui_conversion", "value"));

    path = get("importgui_filename", "string");

    importHeader = %f;

    if hasHeader then
        x = x_mdialog("Create function", ["Function name";"Import header"], ["importdata";"%F"]);
        if isempty(x) then
            return;
        end

        importHeader = evstr(x(2));
    else
        x = x_mdialog("Create function", ["Function name"], ["importdata"]);
        if isempty(x) then
            return;
        end
    end
    
    
    str = [];
    str($+1) = sprintf("clear %s;", x(1));
    str($+1) = "";
    if importHeader then
        str($+1) = sprintf("function [header, data] = %s(filename)", x(1));
        str($+1) = "    header = mgetl(filename, 1);";
        str($+1) = sprintf("    header = csvTextScan(header, ""%s"", ""%s"", ""string"");", delim, decimal);
    else
        str($+1) = sprintf("function [data] = %s(filename)", x(1));
    end
    
    if hasHeader then
        str($+1) = sprintf("    data = csvRead(filename, ""%s"", ""%s"", ""%s"", [], [], [], 1);", delim, decimal, conversion);
    else
        str($+1) = sprintf("    data = csvRead(filename, ""%s"", ""%s"", ""%s"");", delim, decimal, conversion);
    end
    
    str($+1) = "endfunction";
    str($+1) = "";
    
    if importHeader then
        str($+1) = sprintf("[header, data] = importdata(""%s"");", path);
    else
        str($+1) = sprintf("[data] = importdata(""%s"");", path);
    end
    
    clipboard("copy", str);
endfunction
