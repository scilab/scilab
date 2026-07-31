// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function uiimport_cbselect()
    data = get("uiimport", "userdata");
    path = data.path;

    if path == "" then
        path = pwd();
    else
        path = fileparts(path);
    end

    path = uigetfile(["*.txt" "Text files"; "*.csv" "CSV files"], path, "Choose a file", %f);
    if ~isempty(path) then
        data = get("uiimport", "userdata");
        data.path = path;
        data.opts = [];
        data.numheaderlines = [];
        data.filetext = [];
        set("uiimport", "userdata", data);
        set("uiimport", "figure_name", path);
        p = progressionbar("Import data ...");
        p.tag = "uiimport_progressionbar";
        uiimport_dopreview([], [], []);
        if is_handle_valid(p) then
            delete(p);
        end
    end
endfunction
