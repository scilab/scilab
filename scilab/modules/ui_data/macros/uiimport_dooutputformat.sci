// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function uiimport_dooutputformat(colIndex, outputfmt)
    data = get("uiimport", "userdata");
    idx = colIndex + 1;

    if outputfmt == data.outputFormat(idx) then
        return;
    end

    // validate against a real sample of column values
    opts = data.opts;
    vartype = opts.variableTypes(idx);
    inputfmt = opts.inputFormat(idx);

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
            datetime(values, "InputFormat", inputfmt, "OutputFormat", outputfmt);
        else
            duration(values, "InputFormat", inputfmt, "OutputFormat", outputfmt);
        end
    catch
        errclear();
        uiimport_sendtobrowser(struct("type", "error", "message", "The output format """ + outputfmt + """ cannot be applied."));
        uiimport_sendtobrowser(struct("type", "updateformat", "column", colIndex, "outputFormat", data.outputFormat(idx)));
        return;
    end

    data.outputFormat(idx) = outputfmt;
    set("uiimport", "userdata", data);
    uiimport_sendtobrowser(struct("type", "updateformat", "column", colIndex, "outputFormat", data.outputFormat(idx)));
    uiimport_sendtobrowser(struct("type", "refresh"));
endfunction
