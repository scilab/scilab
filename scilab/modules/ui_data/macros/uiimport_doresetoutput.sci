// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function uiimport_doresetoutput(colIndex, fmtRef)
    data = get("uiimport", "userdata");
    data.outputFormat(colIndex + 1) = fmtRef;
    set("uiimport", "userdata", data);

    uiimport_sendtobrowser(struct("type", "updateformat", "column", colIndex, "outputFormat", fmtRef));
    uiimport_sendtobrowser(struct("type", "refresh"));
endfunction
