// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Valentin MULLER
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.


function ret = parquetRead(filename, varargin)
    arguments
        filename {mustBeA(filename, "string"), mustBeScalar}
        varargin
    end

    fname = "parquetRead";

    if ~isfile(filename) then
        error(msprintf(_("%s: Wrong value for input argument #%d: File ""%s"" does not exist.\n"), fname, 1, filename));
    end

    if and(fileext(filename) <> [".parquet", ".arrow"]) then
        error(msprintf(_("%s: Wrong extension for input argument #%d: %s expected.\n"), fname, 1, sci2exp([".parquet", ".arrow"])));
    end

    listArgs = list();
    rowTimes = "";
    isTime = %f;
    outage = "auto";
    rhs = nargin;

    if rhs > 1 then
        if modulo(rhs - 1, 2) <> 0 then
            error(msprintf(_("%s: Options must be specified as key-value pairs.\n"), fname));
        end

        for i = 1:2:rhs-1
            key = varargin(i);

            if type(key) <> 10 then
                error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, i));
            end

            value = varargin(i + 1);
            typ = typeof(value);

            select convstr(key, "l")
            case "rowtimes"
                if and(typ <> ["string", "duration", "datetime"]) then
                    error(msprintf(_("%s: Wrong type for %s argument: Must be in %s.\n"), fname, key, sci2exp(["string", "duration", "datetime"])));
                end

                if typ == "string" then
                    if ~isscalar(value) then
                        error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), fname, key));
                    end
                    rowTimes = value;
                end

                if or(typ == ["duration", "datetime"]) then
                    listArgs($+1) = key;
                    listArgs($+1) = value;
                end

                isTime = %t;

            case "starttime"
                if and(typ <> ["datetime", "duration"]) then
                    error(msprintf(_("%s: Wrong type for %s argument: Must be in %s.\n"), fname, key, sci2exp(["duration", "datetime"])));
                end

                if ~isscalar(value) then
                    error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), fname, key));
                end
                isTime = %t;
                listArgs($+1) = key;
                listArgs($+1) = value;

            case "samplerate"
                if typ <> "constant" then
                    error(msprintf(_("%s: Wrong type for %s argument: Must be in %s.\n"), fname, key, sci2exp("double")));
                end

                if ~isscalar(value) then
                    error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), fname, key));
                end

                isTime = %t;
                listArgs($+1) = key;
                listArgs($+1) = value;

            case "timestep"
                if and(typ <> ["duration", "calendarDuration"]) then
                    error(msprintf(_("%s: Wrong type for %s argument: Must be in %s.\n"), fname, key, sci2exp(["duration", "calendarDuration"])));
                end

                if ~isscalar(value) then
                    error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), fname, key));
                end

                isTime = %t;
                listArgs($+1) = key;
                listArgs($+1) = value;

            case "outputtype"
                if typ <> "string" then
                    error(msprintf(_("%s: Wrong type for %s argument: Must be in %s.\n"), fname, key, sci2exp("string")));
                end

                if ~isscalar(value) then
                    error(msprintf(_("%s: Wrong size for %s argument: scalar expected.\n"), fname, key));
                end

                if and(value <> ["auto", "table", "timeseries"]) then
                    error(msprintf(_("%s: Wrong value for %s argument: %s expected.\n"), fname, key, sci2exp(["auto", "table", "timeseries"])));
                end

                outage = value;
            else
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' not allowed.\n"), fname, i, key));
            end
        end
    end

    [l, varnames, vartypes] = %parquetRead(filename);
    idx = find(vartypes == "datetime");
    if idx <> [] then
        if outage == "timeseries" && ~isTime then
            index = idx(1);
        end

        for i = idx
            l(i) = convertDoubleColumnToDatetime(l(i));
        end
    end

    idx = find(vartypes == "duration");
    if idx <> [] then
        if outage == "timeseries" && ~isTime then
            if exists("index") then
                index = min(index, idx(1));
            else
                index = idx(1);
            end
        end

        for i = idx
            l(i) = milliseconds(l(i));
        end
    end


    if isTime then
        if typeof(rowTimes) == "string" && rowTimes <> "" then
            idx = find(varnames == rowTimes, 1);
            if idx == [] then
                error(msprintf(_("%s: Wrong value for %s argument: A variable time expected.\n"), fname, "RowTimes"));
            end

            nametime = varnames(idx);
            varnames(idx) = [];
            varnames = [nametime, varnames];

            dt = l(idx);
            if ~isduration(dt) & ~isdatetime(dt) then
                error(msprintf(_("%s: Variable Name specified by RowTimes must contain datetime or duration values.\n"), fname));
            end
            l(idx) = null();
            l(0) = dt;
        end

        ret = timeseries(l(:), "VariableNames", varnames, listArgs(:));

    elseif outage == "timeseries" then
        nametime = varnames(index);
        varnames(index) = [];
        varnames = [nametime, varnames];

        dt = l(index);
        l(index) = null();
        l(0) = dt;
        ret = timeseries(l(:), "VariableNames", varnames);
    else
        ret = table(l(:), "VariableNames", varnames);
    end    
endfunction

function convertedColumn = convertDoubleColumnToDatetime(data)

    nrows = length(data);

    convertedColumn = [];

    epoch_datenum = datenum(1970, 1, 1, 0, 0, 0);
    convertedColumn = NaT(nrows, 1);
    mask = ~isnan(data);

    microseconds = data(mask);
    daysz = microseconds / 1000000 / 86400;
    target_datenum = epoch_datenum + daysz;
    convertedColumn(mask) = datetime(target_datenum, "ConvertFrom", "datenum");
endfunction