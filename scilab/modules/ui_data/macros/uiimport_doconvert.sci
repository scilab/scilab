// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

// Convert/format operations: update state and refresh browser cache
// so rows are re-fetched with new transformations applied.
function uiimport_doconvert(colIndex, convert_name)
    data = get("uiimport", "userdata");

    if convert_name == "" || convert_name == "No" then
        data.convert(colIndex + 1) = "";
    else
        data.convert(colIndex + 1) = convert_name;
    end

    set("uiimport", "userdata", data);
    // refresh browser cache to re-fetch with new convert
    uiimport_sendtobrowser(struct("type", "refresh"));
endfunction
