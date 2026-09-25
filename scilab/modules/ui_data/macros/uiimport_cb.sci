// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function uiimport_cb(msg, cb)

    if msg == "loaded" then
        // browser is ready, if a file was provided, parse it
        data = get("uiimport", "userdata");
        if data.path == "" then
            uiimport_cbselect();
        else
            set("uiimport", "figure_name", data.path);
            p = progressionbar("Import data ...");
            p.tag = "uiimport_progressionbar";
            uiimport_dopreview([], [], []);
            if is_handle_valid(p) then
                delete(p)
            end
        end
        return;
    end

    select msg.type
    case "selectfile"
        uiimport_cbselect();
    case "preview"
        uiimport_dopreview(msg.delimiter, msg.decimal, msg.numheaderlines);
    case "fetchrows"
        uiimport_fetchrows(msg.start, msg.count);
    case "sheet"
        data = get("uiimport", "userdata");
        data.sheet = msg.sheet;
        data.filetext = [];
        data.opts = [];
        data.numheaderlines = [];
        set("uiimport", "userdata", data);
        p = progressionbar("Import data ...");
        p.tag = "uiimport_progressionbar";
        uiimport_dopreview([], [], []);
        if is_handle_valid(p) then
            delete(p)
        end
    case "variable"
        select msg.vartype
        case {"table", "timeseries", "cell", "double_matrix", "string_matrix"}
            d = uiimport_dovariable(msg.config, msg.vartype);
            if d == [] then
                return
            end
            execstr(msg.varname + " = resume(d)");
            printf("Imported variable: %s\n", msg.varname);

        case "column_vectors"
            [varnames, d] = uiimport_dovariable(msg.config, msg.vartype);
            for i = 1:size(d)
                execstr(varnames(i) + "= resume(d(i))")
                printf("Imported variable: %s\n", varnames(i));
            end
        end
        
    case "function"
        uiimport_dofunction(msg.config, msg.varname, msg.vartype);
    case "help"
        doc("uiimport");
    case "convert"
        uiimport_doconvert(msg.column, msg.convert);
    case "inputformat"
        uiimport_doinputformat(msg.column, msg.format);
    case "outputformat"
        uiimport_dooutputformat(msg.column, msg.format);
    case "resetinput"
        uiimport_doresetinput(msg.column, msg.format);
    case "resetoutput"
        uiimport_doresetoutput(msg.column, msg.format);
    end

endfunction
