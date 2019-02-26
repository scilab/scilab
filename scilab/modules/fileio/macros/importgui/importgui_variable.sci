function importgui_variable()
    hasHeader = get("importgui_header", "value");
    delim = get("importgui_delim", "userdata")(get("importgui_delim", "value"));
    decimal = get("importgui_decimal", "userdata")(get("importgui_decimal", "value"));
    conversion = get("importgui_conversion", "userdata")(get("importgui_conversion", "value"));

    path = get("importgui_filename", "string");
    if isempty(path) || ~isfile(path) then
        return;
    end

    importHeader = %f;
    
    if hasHeader then
        x = x_mdialog("Import Data", ["Variable name";"Import header";"Header name"], ["data";"%F";"header"]);
        if isempty(x) then
            return;
        end

        importHeader = evstr(x(2));
        if importHeader then
            header = mgetl(path, 1);
            header = csvTextScan(header, delim, decimal, "string");
            str = sprintf("[%s, %s] = resume(data, header);", x(1), x(3));
        else
            str = sprintf("[%s] = resume(data);", x(1));
        end

        dataName = x(1);
        data = csvRead(path, delim, decimal, conversion, [], [], [], 1);
    else
        x = x_mdialog("Import Data", ["Variable name"], ["data"]);
        if isempty(x) then
            return;
        end

        str = sprintf("[%s] = resume(data);", x(1));
        dataName = x(1);
        data = csvRead(path, delim, decimal, conversion);
    end

    execstr(str);
endfunction
