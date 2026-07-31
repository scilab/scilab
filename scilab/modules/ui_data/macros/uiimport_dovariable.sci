// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = uiimport_dovariable(config, vartype)

    data = get("uiimport", "userdata");
    path = data.path;

    if isempty(path) || ~isfile(path) then
        return;
    end

    p = progressionbar("Import variable to workspace...");

    // update state from browser config (0-indexed -> 1-indexed)
    keepcols = config.keepcols + 1;
    varNames = config.varnames;
    rowtimes = config.rowtimes;
    converts = config.converts;
    outputFormats = config.outputFormats;

    opts = data.opts;
    selVarNames = varNames(keepcols);
    selVarNamesRef = opts.variableNames(keepcols);
    selConvert = converts(keepcols);
    selOutputFmt = outputFormats(keepcols);
    vartypes = opts.variableTypes(keepcols);

    convert_fcn = list([], seconds, minutes, hours, days, years);
    convert_names = ["", "seconds", "minutes", "hours", "days", "years"];

    select vartype
    case {"table", "cell"}
        if and(vartypes == "double") then
            // table dans le cas des doubles, on recupere juste les valeurs doubles
            mat = csvTextScan(data.filetext(opts.datalines), opts.delimiter, opts.decimal, "double");
            d = table(mat(:, keepcols), "VariableNames", selVarNames);
        else
            mat = csvTextScan(data.filetext(opts.datalines, :), opts.delimiter, opts.decimal, "string");
            l = list();
            for k = keepcols
                m = mat(:, k);
                fmt = opts.inputFormat(k);
                outputfmt = outputFormats(k);
                select opts.variableTypes(k)
                case "duration"
                    d = duration(%nan) * ones(m);
                    idx = m <> "";
                    d(idx) = duration(m(idx), "InputFormat", fmt);
                    d.format = outputfmt(outputfmt <> "");
                case "datetime"
                    d = NaT(m);
                    idx = m <> "";
                    d(idx) = datetime(m(idx), "InputFormat", fmt);
                    d.format = outputfmt(outputfmt <> "");
                case "double"
                    d = strtod(m)
                    if converts(k) <> "" then
                        d = convert_fcn(converts(k) == convert_names)(d);
                    end
                case "boolean"
                    m = convstr(m, "l");
                    d = members(m, ["t", "true"]) == 1;
                else
                    d = m
                end
                l($+1) = d;
            end

            d = table(l(:), "VariableNames", selVarNames);
            if vartype == "cell" then
                d = table2cell(d);
            end
        end

    case "timeseries"
        if rowtimes == "No" then
            delete(p);
            varargout(1) = [];
            messagebox("Unable to create a variable. You must choose the time reference.", "Warning", "warning", "Ok", "modal");
            return;
        end

        idx = find(rowtimes == selVarNames);
        if selConvert(idx) <> "" then
            jdx = find(selConvert(idx) == convert_names);
            d = readtimeseries(path, opts, "VariableNames", selVarNamesRef, "RowTimes", selVarNamesRef(idx), "ConvertTime", convert_fcn(jdx));
            selConvert(idx) = "";
        else
            if vartypes(idx) == "double" then
                delete(p);
                varargout(1) = [];
                messagebox("Unable to create a variable. You must choose the conversion method.", "Warning", "warning", "Ok", "modal");
                return;
            end
            d = readtimeseries(path, opts, "VariableNames", selVarNamesRef, "RowTimes", selVarNamesRef(idx));
        end
        [a, k] = members(d.Properties.VariableNames, selVarNamesRef);
        selVarNames = selVarNames(k);
        selConvert = selConvert(k);
        selOutputFmt = selOutputFmt(k);

        if or(selVarNamesRef <> selVarNames) then
            d.Properties.VariableNames = selVarNames;
        end

        idx = find(selConvert <> "");
        if idx <> [] then
            for i = idx
                jdx = find(selConvert(i) == convert_names);
                d(selVarNames(i)) = convert_fcn(jdx)(d(selVarNames(i)));
            end
        end

        idx = find(selOutputFmt <> "");
        if idx <> [] then
            for i = idx
                d(selVarNames(i)).format = selOutputFmt(i);
            end
        end
    case "column_vectors"
        d = list();
        varargout(1) = selVarNames;
        if and(vartypes == "double") then
            mat = csvTextScan(data.filetext(opts.datalines), opts.delimiter, opts.decimal, "double");
            for k = keepcols
                d($+1) = mat(:, k);
            end
        else
            mat = csvTextScan(data.filetext(opts.datalines, :), opts.delimiter, opts.decimal, "string");
            for k = keepcols
                m = mat(:, k);
                fmt = opts.inputFormat(k);
                outputfmt = outputFormats(k);
                select opts.variableTypes(k)
                case "duration"
                    dd = duration(%nan) * ones(m);
                    idx = m <> "";
                    dd(idx) = duration(m(idx), "InputFormat", fmt);
                    dd.format = outputfmt(outputfmt <> "");
                case "datetime"
                    dd = NaT(m);
                    idx = m <> "";
                    dd(idx) = datetime(m(idx), "InputFormat", fmt);
                    dd.format = outputfmt(outputfmt <> "");
                case "double"
                    dd = strtod(m);
                    if converts(k) <> "" then
                        dd = convert_fcn(converts(k) == convert_names)(dd);
                    end
                case "boolean"
                    m = convstr(m, "l");
                    dd = members(m, ["t", "true"]) == 1;
                else
                    dd = m
                end
                d($+1) = dd;
            end
        end

    case "double_matrix"
        if and(vartypes == "double") then
            d = csvTextScan(data.filetext(opts.datalines), opts.delimiter, opts.decimal, "double")(:, keepcols);
        else
            mat = csvTextScan(data.filetext(opts.datalines, :), opts.delimiter, opts.decimal, "string");
            d = %nan * ones(size(mat, 1), length(keepcols));
            for i = 1:length(keepcols)
                k = keepcols(i)
                m = mat(:, k);
                fmt = opts.inputFormat(k);
                select opts.variableTypes(k)
                case "duration"
                    d(m <> "", i) = duration(m(m <> ""), "InputFormat", fmt).duration;
                case "datetime"
                    dt = datetime(m(m <> ""), "InputFormat", fmt);
                    d(m <> "", i) = datenum(dt);
                case "double"
                    d(:, i) = strtod(m)
                case "boolean"
                    m = convstr(m, "l");
                    dd = ones(m);
                    isFalse = members(m, ["f", "false"]) == 1;
                    dd(isFalse) = 0;
                    d(:, i) = dd;
                end
            end
        end

    case "string_matrix"
        d = csvTextScan(data.filetext(opts.datalines, :), opts.delimiter, opts.decimal, "string")(:, keepcols);
            
        for i = 1:length(keepcols)
            m = d(:, i);
            k = keepcols(i)
            fmt = opts.inputFormat(k);
            outputfmt = outputFormats(k);
            select vartypes(i)
            case "duration"
                idx = m <> "";
                d(idx, i) = string(duration(m(idx), "InputFormat", fmt, "OutputFormat", outputfmt(outputfmt <> "")));
            case "datetime"
                idx = m <> "";
                d(idx, i) = string(datetime(m(idx), "InputFormat", fmt, "OutputFormat", outputfmt(outputfmt <> "")));
            case "double"
                if selConvert(i) <> "" then
                    d = string(convert_fcn(selConvert(i) == convert_names)(m));
                end
            end
        end
    end    

    delete(p);
    varargout($+1) = d;
endfunction
