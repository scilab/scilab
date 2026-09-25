// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

// Read a range of lines from the CSV, parse them, and send to browser.
// start is 0-indexed (from JS), count is the number of rows requested.
function uiimport_fetchrows(startIdx, count)
    data = get("uiimport", "userdata");
    opts = data.opts;

    if opts == [] || data.filetext == [] then
        return;
    end

    totalDataRows = size(opts.datalines, "*");

    // clamp range
    startIdx = max(0, startIdx);
    endIdx = min(startIdx + count, totalDataRows);
    if startIdx >= endIdx then
        return;
    end

    // index directly into cached data
    if data.filetype == "xlsx" then
        x = data.filetext(opts.datalines(startIdx + 1 : endIdx), :);
    else
        txt = data.filetext(opts.datalines(startIdx + 1 : endIdx));
        try
            x = csvTextScan(txt, opts.delimiter, opts.decimal, "string");
        catch
            return;
        end
    end


    x = uiimport_applyformat(x, opts, data.outputFormat, data.convert);

    uiimport_sendtobrowser(struct("type", "rows", "start", startIdx, "rows", x));
endfunction
