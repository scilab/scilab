// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function uiimport_dopreview(delimiter, decimal, numheaderlines)
    data = get("uiimport", "userdata");
    path = data.path;

    if isempty(path) || ~isfile(path) then
        return;
    end

    // parse numheaderlines from JS (could be double or [])
    if numheaderlines <> [] then
        data.numheaderlines = numheaderlines;
    end

    // Read file once and cache
    if data.filetext == [] then
        data.filetext = mgetl(path);
    end

    // pass cached text to detectImportOptions to avoid re-reading the file
    if delimiter == [] || delimiter == "" then
        // first load: auto-detect
        try
            opts = detectImportOptionsCSV(data.filetext, "", [], data.numheaderlines);
            if opts.variableNames <> [] & size(opts.variableNames, "*") <> size(opts.variableTypes, "*") then
                uiimport_sendtobrowser(struct("type", "error", "clear", %t, "message", "Problem during the import. Choose another delimiter or decimal separator."));
                return;
            end
        catch
            uiimport_sendtobrowser(struct("type", "error", "clear", %t, "message", "Problem during the import. Choose another delimiter or decimal separator or specify the number of lines of header."));
            return;
        end
    else
        // user changed delimiter/decimal
        try
            if decimal == [] || decimal == "" then
                decimal = ".";
            end
            opts = detectImportOptionsCSV(data.filetext, delimiter, decimal, data.numheaderlines);
            if opts.variableNames <> [] & size(opts.variableNames, "*") <> size(opts.variableTypes, "*") then
                uiimport_sendtobrowser(struct("type", "error", "clear", %t, "message", "Problem during the import. Choose another delimiter or decimal separator."));
                return;
            end
        catch
            uiimport_sendtobrowser(struct("type", "error", "clear", %t, "message", "Problem during the import. Choose another delimiter or decimal separator or specify the number of lines of header."));
            return;
        end
    end


    nbrows = size(opts.datalines, "*") + size(opts.header, "*");
    if opts.datalines(1) <> size(opts.header, "*") + 1 then
        nbrows = nbrows + 1;
    end

    if opts.variableNames == [] then
        opts.variableNames = "Var" + string(1:size(opts.variableTypes, "*"));
    end

    nbcols = size(opts.variableNames, "*");
    varnames = opts.variableNames;
    delim = opts.delimiter;
    decim = opts.decimal;
    totalDataRows = size(opts.datalines, "*");

    // Initial batch from cached file text
    initialCount = min(100, totalDataRows);
    initialRows = [];

    if initialCount > 0 then
        txt = data.filetext(opts.datalines(1:initialCount));

        try
            initialRows = csvTextScan(txt, delim, decim, "string");

            if size(initialRows, "c") <> size(varnames, "c") then
                uiimport_sendtobrowser(struct("type", "error", "clear", %t, "message", "Problem during the import. Choose another delimiter or decimal separator."));
                return;
            end
        catch
            uiimport_sendtobrowser(struct("type", "error", "clear", %t, "message", "Problem during the import. Choose another delimiter or decimal separator or specify the number of lines of header."));
            return;
        end
    end

    // update state
    data.opts = opts;
    data.outputFormat = opts.inputFormat;
    data.convert = emptystr(1, nbcols);
    set("uiimport", "userdata", data);

    // send preview data to browser
    preview = struct( ...
        "type", "preview", ...
        "data", struct( ...
            "nbrows", nbrows, ...
            "nbcols", nbcols, ...
            "headerlines", size(opts.header, "*"), ...
            "delimiter", delim, ...
            "decimal", decim, ...
            "varnames", varnames, ...
            "vartypes", opts.variableTypes, ...
            "inputFormats", opts.inputFormat, ...
            "totalDataRows", totalDataRows, ...
            "initialRows", initialRows, ...
            "initialStart", 0));

    uiimport_sendtobrowser(preview);
endfunction
