// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function uiimport_doinputformat(colIndex, inputfmt)
    data = get("uiimport", "userdata");
    opts = data.opts;
    idx = colIndex + 1;

    if inputfmt == opts.inputFormat(idx) then
        return;
    end

    // validate the format against a real sample of column values
    vartype = opts.variableTypes(idx);

    nbSample = min(50, size(opts.datalines, "*"));
    if data.filetype == "xlsx" then
        values = data.filetext(opts.datalines(1:nbSample), idx);
    else
        txt = data.filetext(opts.datalines(1:nbSample));
        %sample = csvTextScan(txt, opts.delimiter, opts.decimal, "string");
        values = %sample(:, idx);
    end

    try
        if vartype == "datetime" then
            datetime(values, "InputFormat", inputfmt);
        else
            duration(values, "InputFormat", inputfmt);
        end
    catch
        errclear();
        uiimport_sendtobrowser(struct("type", "error", "message", "The input format """ + inputfmt + """ cannot be applied."));
        uiimport_sendtobrowser(struct("type", "updateformat", "column", colIndex, "inputFormat", opts.inputFormat(idx)));
        return;
    end

    opts.inputFormat(idx) = inputfmt;
    data.opts = opts;
    set("uiimport", "userdata", data);
    uiimport_sendtobrowser(struct("type", "updateformat", "column", colIndex, "inputFormat", opts.inputFormat(idx)));
    uiimport_sendtobrowser(struct("type", "refresh"));
endfunction
