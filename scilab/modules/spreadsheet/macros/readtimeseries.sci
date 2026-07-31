// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2022 - Dassault Systèmes S.E. - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tt = readtimeseries(varargin)

    sampleRate  = [];
    timeStep = [];
    startTime = duration(0);
    rowTimes = "";
    nodate = %f;
    listArgs = list();
    rhs = nargin;
    names = "";
    method = "";
    sheet = 1;
    rangevalue = "";

    nargs = argn(2);
    if nargs < 1 then
        error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"), fname, 1));
    end

    filename = varargin(1);
    

    if rhs > 2 then
        for i = nargin-1:-2:2
            key = varargin(i);
            if type(key) <> 10 then
                break;
            end

            select convstr(key, "l")
            case "samplerate"
                sampleRate = varargin(i + 1);
                if type(sampleRate) <> 1 then
                    error(msprintf(_("%s: Wrong type for %s argument: double expected.\n"), "readtimeseries", key));
                end
                if ~isscalar(sampleRate) then
                    error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), "readtimeseries", key));
                end
                nodate = %t;
                listArgs($+1) = key;
                listArgs($+1) = sampleRate;

            case "timestep"
                timeStep = varargin(i + 1);
                if and(typeof(timeStep) <> ["duration", "calendarDuration"]) then
                    error(msprintf(_("%s: Wrong type for %s argument: duration or calendarDuration expected.\n"), "readtimeseries", key));
                end
                if ~isscalar(timeStep) then
                    error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), "readtimeseries", key));
                end
                nodate = %t;
                listArgs($+1) = key;
                listArgs($+1) = timeStep;

            case "starttime"
                startTime = varargin(i + 1);
                if and(typeof(startTime) <> ["duration", "datetime"]) then
                    error(msprintf(_("%s: Wrong type for %s argument: duration or datetime expected.\n"), "readtimeseries", key));
                end
                if ~isscalar(startTime) then
                    error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), "readtimeseries", key));
                end
                nodate = %t;
                listArgs($+1) = key;
                listArgs($+1) = startTime;

            case "rowtimes"
                rowTimes = varargin(i + 1);
                if and(typeof(rowTimes) <> ["string", "duration", "datetime"]) then
                    error(msprintf(_("%s: Wrong type for %s argument: string or duration or datetime expected.\n"), "readtimeseries", key));
                end
                if type(rowTimes) == 10 && ~isscalar(rowTimes) then
                    error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), "readtimeseries", key));
                end
                if or(typeof(rowTimes) == ["duration", "datetime"]) then
                    nodate = %t;
                    listArgs($+1) = key;
                    listArgs($+1) = rowTimes;
                end

            case "converttime"
                method = varargin(i + 1);
                if type(method) <> 13 then
                    error(msprintf(_("%s: Wrong type for %s argument: function expected.\n"), "readtimeseries", key));
                end

            case "variablenames"
                names = varargin(i + 1);
                if type(names) <> 10 then
                    error(msprintf(_("%s: Wrong type for %s argument: string expected.\n"), "readtimeseries", key));
                end
            
            case "sheet"
                sheet = varargin(i + 1);
                if type(sheet) == 1 then
                    if sheet <> int(sheet) | sheet < 1 then
                        error(msprintf(_("%s: Wrong value for ""%s"" argument: Must be a positive integer >=1.\n"), fname, key));
                    end
                else
                    if type(sheet) <> 10 then
                        error(msprintf(_("%s: Wrong type for ""%s"" argument: A double or string expected.\n"), fname, key));
                    end
                end

            case "range"
                rangevalue = varargin(i+1);
                if type(rangevalue) == 1 then
                    if and(size(rangevalue) == [2 2]) then
                        rangevalue = %matrix_to_range(rangevalue);
                    else
                        error(msprintf(_("%s: Wrong value for ""%s"" argument: Range matrix must be 2x2 [row1 col1; row2 col2].\n"), fname, key));
                    end
                else
                    if type(rangevalue) <> 10 then
                        error(msprintf(_("%s: Wrong type for ""%s"" argument: Must be a string or a 2x2 matrix.\n"), fname, key));
                    end
                end
            else
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' not allowed.\n"), "readtimeseries", i, key));
            end

            rhs = rhs - 2;
        end
    end

    
    isxlsx = isXlsx(filename);
    if isxlsx then
        f = %xlsxRead(filename, sheet, rangevalue, "string");
    else
        f = mgetl(filename);
    end

    if nargin == 2 || rhs >= 2 then
        opts = varargin(2);
    else
       if ~isxlsx then
            opts = detectImportOptionsCSV(f);
        else
            opts = detectImportOptionsXlsx(f);
        end
    end

    if ~isxlsx then
        mat = csvTextScan(f(opts.datalines, :), opts.delimiter, opts.decimal, "string");//(:,_kk);
    else
        mat = f(opts.datalines, :);
    end

    variableNames = opts.variableNames; 
    variableTypes = opts.variableTypes;
    hasvarnames = %t;

    if variableNames == [] then
        if names <> "" then
            variableNames = names;
        elseif nodate then
            variableNames = ["Var" + string(1:size(variableTypes, "*"))];
        else
            variableNames = ["Time", "Var" + string(1:size(variableTypes, "*")-1)];
        end
        hasvarnames = %f;
    end
    
    fmt = opts.inputFormat;

    if names <> "" then
        [nb, _kk] = members(names, variableNames);
        if and(nb == 0) then
            error(msprintf(_("%s: no matching VariableNames.\n"), "readtimeseries"));
        end
        variableNames = names;
        variableTypes = variableTypes(_kk);
        fmt = fmt(_kk);
    else
        _kk = 1:$;
    end

    idx = [];
    if rowTimes == "" then
        idx = grep(variableTypes, "/^"+["datetime", "duration"]+"$/", "r");
    elseif type(rowTimes) == 10 then
        idx = find(variableNames == rowTimes)
    end

    if idx == [] && ~nodate then
        error(msprintf(_("%s: A variable time expected.\n"), "readtimeseries"));
    end

    mat = mat(:, _kk);
    index = 1;

    if idx <> [] && ~nodate then
        i = idx(1);
        if hasvarnames then
            nametime = variableNames(i);
            variableNames(i) = [];
            variableNames = [nametime, variableNames];
        end

        tmp = variableTypes(i);
        variableTypes(i) = [];
        variableTypes =[tmp, variableTypes];

        tmp = fmt(i);
        fmt(i) = [];
        fmt = [tmp, fmt];
    
        tmp = mat(:, i);
        mat(:, i) = [];
        mat = [tmp, mat]

        index = 2;
    end

    l = list();
    for j = index:size(mat, 2)
        m = mat(:,j)
        select variableTypes(j)
        case "duration"
            d = duration(%nan) * ones(m);
            idx = m <> "";
            d(idx) = duration(m(idx), "InputFormat", fmt(j));
            l($+1) = d;
        case "datetime"
            d = NaT(m);
            idx = m <> "";
            d(idx) = datetime(m(idx), "InputFormat", fmt(j));
            l($+1) = d;
        case "double"
            l($+1) = strtod(m)
        case "boolean"
            m = convstr(m, "l");
            d = members(m, ["t", "true"]) == 1;
            l($+1) = d
        else
            l($+1) = m
        end
    end

    if nodate then
        tt = timeseries(l(:), "VariableNames", variableNames, listArgs(:));
    else
        m = mat(:,1);
        select variableTypes(1)
        case "duration"
            d = duration(%nan) * ones(m);
            idx = m <> "";
            d(idx) = duration(m(idx));
        case "datetime"
            d = NaT(m);
            idx = m <> "";
            d(idx) = datetime(m(idx), "InputFormat", fmt(1));
        case "double"
            d = method(strtod(m));
        else
            error(msprintf(_("%s: Wrong type for the time column: ''duration'' or ''datetime'' expected.\n"), "readtimeseries"));
        end

        tt = timeseries(d, l(:), "VariableNames", variableNames)
    end
    tt.props.variableDescriptions = tt.props.variableNames;
endfunction
