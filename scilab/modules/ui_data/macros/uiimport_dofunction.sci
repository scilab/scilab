// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function uiimport_dofunction(config, varname, vartype)
    data = get("uiimport", "userdata");
    path = data.path;

    // update state from browser config
    keepcols = config.keepcols + 1;
    varNames = config.varnames;
    rowtimes = config.rowtimes;
    converts = config.converts;
    formatRef = config.formatRef;
    outputFormats = config.outputFormats;

    opts = data.opts;
    selVarNames = varNames(keepcols);
    selVarNamesRef = opts.variableNames(keepcols);
    selConvert = converts(keepcols);
    selOutputFmt = outputFormats(keepcols);
    variableTypes = opts.variableTypes(keepcols);

    x = "import_" + varname;
    filename = fullfile(fileparts(path), x + ".sce");

    if isfile(filename) then
        m = messagebox("This file exists already. Do you want to overwrite it ?", "Warning", "warning", ["Yes", "No"], "modal");
        if m == 1 then
            deletefile(filename);
        else
            return;
        end
    end

    str = [];
    if vartype == "column_vectors" then
        str($+1) = sprintf("clear %s %s;", varname, strcat(selVarNames, " "));
        str($+1) = "";
        str($+1) = sprintf("function [%s] = %s(filename)", strcat(selVarNames, ","), x(1));
        varname = "data";
    else
        str($+1) = sprintf("clear %s;", varname);
        str($+1) = "";
        str($+1) = sprintf("function [%s] = %s(filename)", varname, x(1));
    end

    p = "";
    if data.numheaderlines <> [] then
        p = sprintf(", ""NumHeaderLines"", %s", sci2exp(data.numheaderlines));
    end

    if opts.decimal <> [] then
        str($+1) = sprintf("    opts = detectImportOptions(filename, ""Delimiter"", ""%s"", ""Decimal"", ""%s""" + p + ");", opts.delimiter, opts.decimal);
    else
        str($+1) = sprintf("    opts = detectImportOptions(filename, ""Delimiter"", ""%s""" + p + ");", opts.delimiter);
    end

    if or(opts.inputFormat <> formatRef) then
        str($+1) = sprintf("    opts.inputFormat = %s;", sci2exp(opts.inputFormat));
    end

    select vartype
    case "string_matrix"
        str($+1) = sprintf("    %s = readtable(filename, opts, ""VariableNames"", %s);", varname, sci2exp(selVarNamesRef));

    case {"table", "column_vectors", "cell", "double_matrix"}
        str($+1) = sprintf("    %s = readtable(filename, opts, ""VariableNames"", %s);", varname, sci2exp(selVarNamesRef));
    case "timeseries"
        idx = find(rowtimes == selVarNames);
        if selConvert(idx) <> "" then
            str($+1) = sprintf("    %s = readtimeseries(filename, opts, ""VariableNames"", %s, ""RowTimes"", ""%s"", ""ConvertTime"", %s);", varname, sci2exp(selVarNamesRef), selVarNamesRef(idx), selConvert(idx));
            selConvert(idx) = "";
        else
            if variableTypes(idx) == "double" then
                messagebox("Unable to create a variable. You must choose the conversion method.", "Warning", "warning", "Ok", "modal");
                return;
            end
            str($+1) = sprintf("    %s = readtimeseries(filename, opts, ""VariableNames"", %s, ""RowTimes"", ""%s"");", varname, sci2exp(selVarNamesRef), selVarNamesRef(idx));
        end
        variableNames = selVarNamesRef;
        vartime = variableNames(idx);
        variableNames(idx) = [];
        variableNames = [vartime variableNames];
        [a, k] = members(variableNames, selVarNamesRef);
        selVarNames = selVarNames(k);
        selConvert = selConvert(k);
        selOutputFmt = selOutputFmt(k);
    end

    if or(selVarNamesRef <> selVarNames) then
        str($+1) = sprintf("    %s.Properties.VariableNames = %s;", varname, sci2exp(selVarNames));
    end

    idx = find(selConvert <> "");
    if idx <> [] then
        for i = idx
            str($+1) = sprintf("    %s(%s) = %s(%s(%s));", varname, sci2exp(selVarNames(i)), selConvert(i), varname, sci2exp(selVarNames(i)));
        end
    end

    idx = find(selOutputFmt <> "");
    if idx <> [] then
        for i = idx
            str($+1) = sprintf("    %s(%s).format = ""%s"";", varname, sci2exp(selVarNames(i)), selOutputFmt(i));
        end
    end

    if vartype == "double_matrix" then
        idx = find(variableTypes <> "double");
        if idx <> [] then
            for i = idx
                name = sci2exp(selVarNames(i));
                select variableTypes(i)
                case "duration"
                    str($+1) = sprintf("    %s(%s) = milliseconds(%s(%s));", varname, name, varname, name);
                case "boolean"
                    str($+1) = sprintf("    %s(%s) = bool2s(%s(%s));", varname, name, varname, name);
                case "datetime"
                    str($+1) = sprintf("    %s(%s) = datenum(%s(%s));", varname, name, varname, name);
                case "string"
                    str($+1) = sprintf("    %s(%s) = %s * ones(%s(%s));", varname, name, "%nan", varname, name);
                end
            end
        end
        str($+1) = sprintf("    %s = table2matrix(%s);", varname, varname);

    elseif vartype == "string_matrix" then
        idx = find(variableTypes <> "string");
        if idx <> [] then
            for i = idx
                name = sci2exp(selVarNames(i));
                str($+1) = sprintf("    %s(%s) = string(%s(%s));", varname, name, varname, name);
            end
        end
        str($+1) = sprintf("    %s = table2matrix(%s);", varname, varname);
    elseif vartype == "cell" then
        str($+1) = sprintf("    %s = table2cell(%s);", varname, varname);
    elseif vartype == "column_vectors" then
        for i = 1:length(keepcols)
            str($+1) = sprintf("    %s = %s(""%s"");", selVarNames(i), varname, selVarNames(i));
        end
    end

    str($+1) = "endfunction";
    str($+1) = "";

    if vartype == "column_vectors" then
        str($+1) = sprintf("[%s] = %s(""%s"");", strcat(selVarNames, ","), x(1), path);
    else
        str($+1) = sprintf("%s = %s(""%s"");", varname, x(1), path);
    end

    mputl(str, filename);
    scinotes(filename);
endfunction
