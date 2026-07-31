// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

// Reformat datetime/duration columns of a string preview matrix according to
// the per-column input/output formats stored in opts and outputFormat.
function rows = uiimport_applyformat(rows, opts, outputFormat, convert)
    if rows == [] then
        return;
    end

    vartypes = opts.variableTypes;
    inputFormats = opts.inputFormat;
    nbcols = size(vartypes, "*");

    for c = 1:nbcols
        infmt = inputFormats(c);
        outfmt = outputFormat(c);
        convert_to = convert(c);
        if (outfmt == "" || outfmt == infmt) && convert_to == "" then
            continue;
        end
        try
            if vartypes(c) == "double" && convert_to <> "" then
                idx = find(rows(:, c) == "");
                rows(idx, c) = "%nan";
                execstr("obj = " + convert_to + "([" + strcat(rows(:, c), ";") + "])");
                rows(:, c) = string(obj);
            elseif vartypes(c) == "datetime" then
                obj = datetime(rows(:, c), "InputFormat", infmt, "OutputFormat", outfmt);
                rows(:, c) = string(obj);
            elseif vartypes(c) == "duration" then
                obj = duration(rows(:, c), "InputFormat", infmt, "OutputFormat", outfmt);
                rows(:, c) = string(obj);
            end
        catch
            errclear();
        end
    end
endfunction
